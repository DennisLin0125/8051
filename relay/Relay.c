/*-----------------------------------------------
  名稱：獨立按鍵控制繼電器開關
  編寫：dennis
  日期：2020.10.5
  修改：無
  內容：對應的繼電器介面需用杜邦線連線到uln2003繼電器控制端
        
------------------------------------------------*/
#include<reg52.h> //包含標頭檔案，一般情況不需要改動，標頭檔案包含特殊功能暫存器的定義

#define KeyPort P3

#define DataPort P0 //定義數據埠 程式中遇到DataPort 則用P0 替換
sbit LATCH1=P2^2;//定義鎖存使能埠 段鎖存
sbit LATCH2=P2^3;//                 位鎖存

sbit RELAY1 = P1^1;//定義繼電器訊號輸出埠1
sbit RELAY2 = P1^2;//定義繼電器訊號輸出埠2

unsigned char code dofly_DuanMa[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};// 顯示段碼值0~9
unsigned char code dofly_WeiMa[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//分別對應相應的數碼管點亮,即位碼
unsigned char TempData[8]; //儲存顯示值的全域性變數
/*------------------------------------------------
                  函式聲明
------------------------------------------------*/
void DelayUs2x(unsigned char t);//us級延時函式聲明 
void DelayMs(unsigned char t); //ms級延時
void Display(unsigned char FirstBit,unsigned char Num);//數碼管顯示函式
unsigned char KeyScan(void);//鍵盤掃瞄
void Init_Timer0(void);//定時器初始化
/*------------------------------------------------
                    主函式
------------------------------------------------*/
void main (void)
{
	unsigned char num;                  

	Init_Timer0();//初始化定時器0，主要用於數碼管動態掃瞄

	TempData[0]=0x50; //'r'
	TempData[1]=0x79; //'E'
	TempData[2]=0x38; //'L'
	TempData[3]=0x06; //'1'

	TempData[5]=0x3F; //'O'
	TempData[6]=0x71; //'F'
	TempData[7]=0x71; //'F'
	while (1)         //主循環
	{
		num=KeyScan();    //循環呼叫按鍵掃瞄
		switch(num)
		{
			//繼電器1吸合狀態,數碼管顯示執行狀態
			case 1:RELAY1=!RELAY1;
			TempData[0]=0x50; //'r'
			TempData[1]=0x79; //'E'
			TempData[2]=0x38; //'L'
			TempData[3]=0x06; //'1'

			if(RELAY1)
			{
				TempData[5]=0x3F; //'O'
				TempData[6]=0x54; //'n'
				TempData[7]=0;
			}
			else
			{
				TempData[5]=0x3F; //'O'
				TempData[6]=0x71; //'F'
				TempData[7]=0x71; //'F'
			}
			break;
			//繼電器2吸合狀態,數碼管顯示執行狀態
			case 2:RELAY2=!RELAY2;
			TempData[0]=0x50; //'r'
			TempData[1]=0x79; //'E'
			TempData[2]=0x38; //'L'
			TempData[3]=0x5B; //'2'

			if(RELAY2)
			{
				TempData[5]=0x3F; //'O'
				TempData[6]=0x54; //'n'
				TempData[7]=0;
			}
			else
			{
				TempData[5]=0x3F; //'O'
				TempData[6]=0x71; //'F'
				TempData[7]=0x71; //'F'
			}
			break;
			default:break;
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
 輸入參數 FirstBit 表示需要顯示的第一位，如賦值2表
 示從第三個數碼管開始顯示，如輸入0表示從第一個顯示。
 Num表示需要顯示的位數，如需要顯示99兩位數值則該值
 輸入2
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

	Display(0,8);                // 呼叫數碼管掃瞄

}

/*------------------------------------------------
按鍵掃瞄函式，返回掃瞄鍵值
------------------------------------------------*/
unsigned char KeyScan(void)
{
	unsigned char keyvalue;
	if(KeyPort!=0xff)
	{
		DelayMs(10);
		if(KeyPort!=0xff)
		{
			keyvalue=KeyPort;
			while(KeyPort!=0xff);
			switch(keyvalue)
			{
				case 0xfe:return 1;break;
				case 0xfd:return 2;break;
				case 0xfb:return 3;break;
				case 0xf7:return 4;break;
				case 0xef:return 5;break;
				case 0xdf:return 6;break;
				case 0xbf:return 7;break;
				case 0x7f:return 8;break;
				default:return 0;break;
			}
		}
	}
	return 0;
}

