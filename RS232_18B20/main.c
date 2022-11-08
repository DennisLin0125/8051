/*-----------------------------------------------
  名稱：DS18b20數碼管顯示 
  論壇：www.doflye.net
  編寫：dennis
  日期：2020.10.10
  修改：無
  內容：顯示格式 符號 xxx.x C 
        可以顯示負溫度
        數碼管顯示同時通過串列埠發送到電腦端顯示
        使用dofly配套的測溫軟體 晶振使用11.0592M
------------------------------------------------*/
#include<reg52.h> //包含標頭檔案，一般情況不需要改動，標頭檔案包含特殊功能暫存器的定義
#include<stdio.h>
#include "18b20.h"

#define DataPort P0 //定義數據埠 程式中遇到DataPort 則用P0 替換

sbit LATCH1=P2^2;//定義鎖存使能埠 段鎖存
sbit LATCH2=P2^3;//                 位鎖存

bit ReadTempFlag;//定義讀時間標誌

unsigned char code dofly_DuanMa[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};// 顯示段碼值0~9
unsigned char code dofly_WeiMa[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//分別對應相應的數碼管點亮,即位碼
unsigned char TempData[8]; //儲存顯示值的全域性變數


void Display(unsigned char FirstBit,unsigned char Num);//數碼管顯示函式
void Init_Timer0(void);//定時器初始化
/*------------------------------------------------
              串列埠通訊初始化
------------------------------------------------*/
void UART_Init(void)
{
	SCON  = 0x50;		        // SCON: 模式 1, 8-bit UART, 使能接收  
	TMOD |= 0x20;               // TMOD: timer 1, mode 2, 8-bit 重灌
	TH1   = 0xFD;               // TH1:  重灌值 9600 波特率 晶振 11.0592MHz  
	TR1   = 1;                  // TR1:  timer 1 打開                         
	//EA    = 1;                  //打開總中斷
	//ES    = 1;                  //打開串列埠中斷
	TI =1;
}
/*------------------------------------------------
                    主函式
------------------------------------------------*/
void main (void)
{                  
	unsigned int TempH,TempL,temp;
	Init_Timer0();
	UART_Init();
	while (1)         //主循環
	{
		if(ReadTempFlag==1)
		{
			ReadTempFlag=0;
			temp=ReadTemperature();
			if(temp&0x8000)
			{
				TempData[0]=0x40;//負號標誌
				temp=~temp;  // 取反加1
				temp +=1;
			}
			else
				TempData[0]=0;

			TempH=temp>>4;
			TempL=temp&0x0F;
			TempL=TempL*6/10;//小數近似處理
			printf("%d.%d\n",TempH,TempL);
			
			if(TempH/100==0)
				TempData[1]=0;
			else
				TempData[1]=dofly_DuanMa[TempH/100]; //十位溫度
			
			if((TempH/100==0)&&((TempH%100)/10==0))//消隱
				TempData[2]=0;
			else
				TempData[2]=dofly_DuanMa[(TempH%100)/10]; //十位溫度
			
			TempData[3]=dofly_DuanMa[(TempH%100)%10]|0x80; //個位溫度,帶小數點
			TempData[4]=dofly_DuanMa[TempL];
			TempData[6]=0x39;         //顯示C符號
		}	
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
	if(num==300)        //
	{
		num=0;
		ReadTempFlag=1; //讀標誌位置1
	}
}


