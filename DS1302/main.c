/*-----------------------------------------------
  名稱：DS1302時鐘數碼管顯示
  論壇：www.doflye.net
  編寫：dennis
  日期：2020.10.8
  修改：無
  內容：DS1302實時時鐘數碼管顯示，時間和日期切換顯示 按下按鍵，循環切換 
        時間格式xx-xx-xx
		  日期格式xx-xx-xx
		  周秒格式-x-   xx
------------------------------------------------*/
#include<reg52.h> //包含標頭檔案，一般情況不需要改動，標頭檔案包含特殊功能暫存器的定義
#include "ds1302.h"

#define KeyPort P3 //定義按鍵埠

#define DataPort P0 //定義數據埠 程式中遇到DataPort 則用P0 替換

sbit LATCH1=P2^2;//定義鎖存使能埠 段鎖存
sbit LATCH2=P2^3;//                 位鎖存

bit ReadTimeFlag;//定義讀時間標誌

unsigned char code dofly_DuanMa[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};// 顯示段碼值0~9
unsigned char code dofly_WeiMa[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//分別對應相應的數碼管點亮,即位碼
unsigned char TempData[8]; //儲存顯示值的全域性變數

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
	unsigned char num,displaynum;                  

	Init_Timer0();
	Ds1302_Init();
	Ds1302_Write_Time();
	while (1)         //主循環
	{
		num=KeyScan();   //按鍵掃瞄
		if(num==1)
		{
			displaynum++;
			if(displaynum==3)
				displaynum=0;
		}
		if(ReadTimeFlag==1)
		{
			ReadTimeFlag=0;
			Ds1302_Read_Time();
			if(displaynum==0) //顯示時間
			{
				TempData[0]=dofly_DuanMa[time_buf1[4]/10];//時	//數據的轉換，
				TempData[1]=dofly_DuanMa[time_buf1[4]%10];//因我們採用數碼管0~9的顯示,將數據分開
				TempData[2]=0x40;					    	        //加入"-"
				TempData[3]=dofly_DuanMa[time_buf1[5]/10];//分
				TempData[4]=dofly_DuanMa[time_buf1[5]%10];
				TempData[5]=0x40;
				TempData[6]=dofly_DuanMa[time_buf1[6]/10];//秒
				TempData[7]=dofly_DuanMa[time_buf1[6]%10];	
			} 
			else if(displaynum==1)//顯示日期
			{ 
				TempData[0]=dofly_DuanMa[time_buf1[1]/10];//年			
				TempData[1]=dofly_DuanMa[time_buf1[1]%10];
				TempData[2]=0x40;					    	        //加入"-"
				TempData[3]=dofly_DuanMa[time_buf1[2]/10];//月
				TempData[4]=dofly_DuanMa[time_buf1[2]%10];
				TempData[5]=0x40;
				TempData[6]=dofly_DuanMa[time_buf1[3]/10];//日
				TempData[7]=dofly_DuanMa[time_buf1[3]%10];	
			}
			else if(displaynum==2)//顯示周  秒
			{
				TempData[0]=0x40;				
				TempData[1]=dofly_DuanMa[time_buf1[7]%10];//周
				TempData[2]=0x40;	//加入"-"
				TempData[3]=0;
				TempData[4]=0;
				TempData[5]=0;
				TempData[6]=dofly_DuanMa[time_buf1[6]/10];//秒
				TempData[7]=dofly_DuanMa[time_buf1[6]%10];	
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
	static unsigned int num;
	TH0=(65536-2000)/256;		  //重新賦值 2ms
	TL0=(65536-2000)%256;

	Display(0,8);       // 呼叫數碼管掃瞄
	num++;
	if(num==50)        //大致100ms
	{
		num=0;
		ReadTimeFlag=1; //讀標誌位置1
	}
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

