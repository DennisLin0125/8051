/*-----------------------------------------------
  名稱：矩陣鍵盤模擬密碼鎖
  論壇：www.doflye.net
  編寫：dennis
  日期：2020.9.22
  修改：無
  內容：模擬密碼鎖，密碼固化到ROM 不能通過按鍵輸入更改
------------------------------------------------*/
#include<reg52.h> //包含標頭檔案，一般情況不需要改動，標頭檔案包含特殊功能暫存器的定義


#define DataPort P0 //定義數據埠 程式中遇到DataPort 則用P0 替換
#define KeyPort P1
sbit LATCH1=P2^2;//定義鎖存使能埠 段鎖存
sbit LATCH2=P2^3;//                 位鎖存

unsigned char code dofly_DuanMa[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,
		                  	         0x77,0x7c,0x39,0x5e,0x79,0x71};// 顯示段碼值0~F
unsigned char code dofly_WeiMa[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//分別對應相應的數碼管點亮,即位碼
unsigned char TempData[10]; //儲存顯示值的全域性變數
unsigned char code password[8]={1,2,3,4,5,6,7,8};
//可以更改此密碼做多組測試

void DelayUs2x(unsigned char t);//us級延時函式聲明 
void DelayMs(unsigned char t); //ms級延時
void Display(unsigned char FirstBit,unsigned char Num);//數碼管顯示函式
unsigned char KeyScan(void);//鍵盤掃瞄
unsigned char KeyPro(void);
void Init_Timer0(void);//定時器初始化
/*------------------------------------------------
                    主函式
------------------------------------------------*/
void main (void)
{
	unsigned char num,i,j;                  
	unsigned char temp[8];
	bit Flag;
	Init_Timer0();

	while (1)         //主循環
	{
		num=KeyPro();
		if(num!=0xff)
		{
			if(i==0)
			{
				for(j=0;j<8;j++)//清屏
					TempData[j]=0;
			}
			if(i<8)
			{
				temp[i]=dofly_DuanMa[num];//把按鍵值輸入到臨時陣列中
				for(j=0;j<=i;j++)         //通過一定順序把臨時陣列中	
					TempData[7-i+j]=temp[j];//的值賦值到顯示緩衝區，從右往左輸入
			}
			i++;   //輸入數值累加
			if(i==9)//正常等於8即可，由於我們需要空一個用於清屏，
			//清屏時的按鍵不做輸入值
			{
				i=0;
				Flag=1;//先把比較位置1
				for(j=0;j<8;j++)//循環比較8個數值，如果有一個不等 則最終Flag值為0
					Flag=Flag&&(temp[j]==dofly_DuanMa[password[j]]);
				//比較輸入值和已有密碼
				for(j=0;j<8;j++)//清屏
					TempData[j]=0;
				if(Flag)//如果比較全部相同，標誌位置1
				{
					TempData[0]=0x3f; // "o"
					TempData[1]=0x73; // "p"
					TempData[2]=0x79; // "E"
					TempData[3]=0x54; // "n"
					//說明密碼正確，輸入對應操作 顯示"open"
				}
				else 
				{
					TempData[0]=0x79; // "E"
					TempData[1]=0x50; // "r"
					TempData[2]=0x50; // "r"
					//否則顯示"Err"
				}
			}
		}	
	}
}
/*------------------------------------------------
 uS延時函式，含有輸入參數 unsigned char t，無返回值
 unsigned char 是定義無符號字元變數，其值的範圍是
 0~255 這裡使用晶振12M，精確延時請使用匯編,大致延時
 長度如下 T=tx2+5 uS 
------------------------------------------------*/
void DelayUs2x(unsigned char t)
{   
	while(--t);
}
/*------------------------------------------------
 mS延時函式，含有輸入參數 unsigned char t，無返回值
 unsigned char 是定義無符號字元變數，其值的範圍是
 0~255 這裡使用晶振12M，精確延時請使用匯編
------------------------------------------------*/
void DelayMs(unsigned char t)
{
     
	while(t--)
	{
		//大致延時1mS
		DelayUs2x(245);
		DelayUs2x(245);
	}
}
/*------------------------------------------------
 顯示函式，用於動態掃瞄數碼管
 輸入參數 FirstBit 表示需要顯示的第一位，如賦值2表示從第三個數碼管開始顯示
 如輸入0表示從第一個顯示。
 Num表示需要顯示的位數，如需要顯示99兩位數值則該值輸入2
------------------------------------------------*/
void Display(unsigned char FirstBit,unsigned char Num)
{
	static unsigned char i=0;


	DataPort=0;   //清空數據，防止有交替重影
	LATCH1=1;     //段鎖存
	LATCH1=0;

	DataPort=dofly_WeiMa[i+FirstBit]; //取位碼 
	LATCH2=1;     //位鎖存
	LATCH2=0;

	DataPort=TempData[i]; //取顯示數據，段碼
	LATCH1=1;     //段鎖存
	LATCH1=0;

	i++;
	if(i==Num)
	i=0;


}
/*------------------------------------------------
                    定時器初始化子程式
------------------------------------------------*/
void Init_Timer0(void)
{
	TMOD |= 0x01;	  //使用模式1，16位定時器，使用"|"符號可以在使用多個定時器時不受影響		     
	//TH0=0x00;	      //給定初值
	//TL0=0x00;
	EA=1;            //總中斷打開
	ET0=1;           //定時器中斷打開
	TR0=1;           //定時器開關打開
}
/*------------------------------------------------
                 定時器中斷子程式
------------------------------------------------*/
void Timer0_isr(void) interrupt 1 
{
	TH0=(65536-2000)/256;		  //重新賦值 2ms
	TL0=(65536-2000)%256;

	Display(0,8);       // 呼叫數碼管掃瞄

}

/*------------------------------------------------
按鍵掃瞄函式，返回掃瞄鍵值
------------------------------------------------*/
unsigned char KeyScan(void)  //鍵盤掃瞄函式，使用行列反轉掃瞄法
{
	unsigned char cord_h,cord_l;//行列值中間變數
	KeyPort=0x0f;            //行線輸出全為0
	cord_h=KeyPort&0x0f;     //讀入列線值
	if(cord_h!=0x0f)    //先檢測有無按鍵按下
	{
		DelayMs(10);        //去抖
		if((KeyPort&0x0f)!=0x0f)
		{
			cord_h=KeyPort&0x0f;  //讀入列線值
			KeyPort=cord_h|0xf0;  //輸出目前列線值
			cord_l=KeyPort&0xf0;  //讀入行線值

			while((KeyPort&0xf0)!=0xf0);//等待鬆開並輸出

			return(cord_h+cord_l);//鍵盤最後組合碼值
		}
	}
	return(0xff);     //返回該值
}
/*------------------------------------------------
按鍵值處理函式，返回掃鍵值
------------------------------------------------*/
unsigned char KeyPro(void)
{
	switch(KeyScan())
	{
		case 0x7e:return 0;break;//0 按下相應的鍵顯示相對應的碼值
		case 0x7d:return 1;break;//1
		case 0x7b:return 2;break;//2
		case 0x77:return 3;break;//3
		case 0xbe:return 4;break;//4
		case 0xbd:return 5;break;//5
		case 0xbb:return 6;break;//6
		case 0xb7:return 7;break;//7
		case 0xde:return 8;break;//8
		case 0xdd:return 9;break;//9
		case 0xdb:return 10;break;//a
		case 0xd7:return 11;break;//b
		case 0xee:return 12;break;//c
		case 0xed:return 13;break;//d
		case 0xeb:return 14;break;//e
		case 0xe7:return 15;break;//f
		default:return 0xff;break;
	}
}