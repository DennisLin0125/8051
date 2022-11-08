/*-----------------------------------------------
  名稱：步進電機
  論壇：www.doflye.net
  編寫：dennis
  日期：2020.10.5
  修改：無
  內容：本程式用於測試4相步進電機常規驅動 2個按鍵 調節速度 
        使用1-2相勵磁
		  數碼管顯示 1-5速度等級，數字越大，速度越小
		  將P3接至獨立按鍵
 ------------------------------------------------*/

#include <reg52.h>

#define KeyPort P3
#define DataPort P0 //定義數據埠 程式中遇到DataPort 則用P0 替換
sbit LATCH1=P2^2;//定義鎖存使能埠 段鎖存
sbit LATCH2=P2^3;//                 位鎖存

unsigned char code dofly_DuanMa[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};// 顯示段碼值0~9
unsigned char code dofly_WeiMa[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//分別對應相應的數碼管點亮,即位碼
unsigned char TempData[8]; //儲存顯示值的全域性變數

sbit A1=P1^0; //定義步進電機連線埠
sbit B1=P1^1;
sbit C1=P1^2;
sbit D1=P1^3;


#define Coil_AB1 {A1=1;B1=1;C1=0;D1=0;}//AB相通電，其他相斷電
#define Coil_BC1 {A1=0;B1=1;C1=1;D1=0;}//BC相通電，其他相斷電
#define Coil_CD1 {A1=0;B1=0;C1=1;D1=1;}//CD相通電，其他相斷電
#define Coil_DA1 {A1=1;B1=0;C1=0;D1=1;}//D相通電，其他相斷電
#define Coil_A1 {A1=1;B1=0;C1=0;D1=0;}//A相通電，其他相斷電
#define Coil_B1 {A1=0;B1=1;C1=0;D1=0;}//B相通電，其他相斷電
#define Coil_C1 {A1=0;B1=0;C1=1;D1=0;}//C相通電，其他相斷電
#define Coil_D1 {A1=0;B1=0;C1=0;D1=1;}//D相通電，其他相斷電
#define Coil_OFF {A1=0;B1=0;C1=0;D1=0;}//全部斷電

unsigned char Speed=1;
bit StopFlag;
void Display(unsigned char FirstBit,unsigned char Num);
void Init_Timer0(void);
unsigned char KeyScan(void);
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
                    主函式
------------------------------------------------*/
main()
{
	unsigned int i=512;//旋轉一週時間
	unsigned char num;
	Init_Timer0();
	Coil_OFF
	while(1)  //正向
	{ 
		num=KeyScan();    //循環呼叫按鍵掃瞄
		if(num==1)//第一個按鍵,速度等級增加
		{
			if(Speed<5) Speed++;	
		}	
		else if(num==2)//第二個按鍵，速度等級減小
		{
			if(Speed>1) Speed--;
		}	

		TempData[1]=dofly_DuanMa[Speed%10];
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
	PT0=1;           //優先順序打開
}
/*------------------------------------------------
                 定時器中斷子程式
------------------------------------------------*/
void Timer0_isr(void) interrupt 1 
{
	static unsigned char times,i;
	TH0=(65536-1000)/256;		  //重新賦值 1ms
	TL0=(65536-1000)%256;

	Display(0,8);

	if(times==Speed)
	{
		times=0;
		switch(i)
		{
			case 0:Coil_A1;i++;break;
			case 1:Coil_AB1;i++;break;
			case 2:Coil_B1;i++;break;
			case 3:Coil_BC1;i++;break;
			case 4:Coil_C1;i++;break;
			case 5:Coil_CD1;i++;break;
			case 6:Coil_D1;i++;break;
			case 7:Coil_DA1;i++;break;
			case 8:i=0;break;
			default:break;
		}
	}
	else
		times++;
 
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

