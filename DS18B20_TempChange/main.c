/*-----------------------------------------------
  名稱：DS18b20 溫度檢測液晶顯示
  論壇：www.doflye.net
  編寫：dennis
  日期：2020.10.9
  修改：無
  內容：
------------------------------------------------*/
#include<reg52.h> //包含標頭檔案，一般情況不需要改動，標頭檔案包含特殊功能暫存器的定義
#include<stdio.h>
#include "18b20.h"
#include "1602.h"
#include "delay.h"

#define KeyPort P3 //定義按鍵埠

bit ReadTempFlag;//定義讀時間標誌

void Init_Timer0(void);//定時器初始化
unsigned char KeyScan(void);//鍵盤掃瞄
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
	TI=1;
}
/*------------------------------------------------
                    主函式
------------------------------------------------*/
void main (void)
{                  
	int temp,tempH=50,tempL=1;
	float temperature;
	unsigned char TempFlag=0;
	char displaytemp[16],num;//定義顯示區域臨時儲存陣列

	LCD_Init();           //初始化液晶
	DelayMs(20);          //延時有助於穩定
	LCD_Clear();          //清屏
	Init_Timer0();
	UART_Init();
	Lcd_User_Chr();       //寫入自定義字元


	while (1)         //主循環
	{
		num=KeyScan();
		switch(num)
		{
			case 1:
				if(tempH<127)
					tempH++;
					break;
			case 2:
				if(tempH>-55)
					tempH--;
					break;
			case 3:
				if(tempL<127)
					tempL++;
					break;
			case 4:
				if(tempL>-55)
					tempL--;
					break;
			default:break;
		}

		switch(TempFlag)
		{
			case 0:    //輸出溫度上限下限
			sprintf(displaytemp,"H.%3d  L.%3d ",tempH,tempL);
			  LCD_Write_String(0,1,displaytemp);//顯示第二行
			break;
			case 1:LCD_Write_String(0,1,"over  tempH    ");break;
			case 2:LCD_Write_String(0,1,"under  tempL   ");break;
			default:break;
		}
		if(ReadTempFlag==1)
		{
			ReadTempFlag=0;
			temp=ReadTemperature();


			temperature=temp*0.0625;

			temp>>=4;
			if(temp>tempH)
				TempFlag=1;  //高於高溫標誌
			else if(temp<tempL)
				TempFlag=2;  //低於低溫標誌
			else
				TempFlag=0;   //正常顯示標誌



			sprintf(displaytemp,"Temp  %6.2f ",temperature);//列印溫度值
			LCD_Write_String(0,0,displaytemp);//顯示第一行
			LCD_Write_Char(13,0,0x01);//寫入溫度右上角點
			LCD_Write_Char(14,0,'C'); //寫入字元C
		}	
	}
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

	num++;
	if(num==400)        //
	{
		num=0;
		ReadTempFlag=1; //讀標誌位置1
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


