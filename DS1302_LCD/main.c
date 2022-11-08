/*-----------------------------------------------
  名稱：DS1302時鐘數碼管顯示 串列埠更新
  論壇：www.doflye.net
  編寫：dennis
  日期：2020.10.9
  修改：無
  內容：液晶顯示時間
		通過dofly自帶的串列埠除錯軟體，打開串列埠，波特率預設9600，點選更新時間即可，如果不行，按下開發板復位重新更新
------------------------------------------------*/
#include<reg52.h> //包含標頭檔案，一般情況不需要改動，標頭檔案包含特殊功能暫存器的定義
#include <stdio.h>
#include "ds1302.h"
#include "delay.h"
#include "1602.h"

bit ReadTimeFlag;//定義讀時間標誌
bit SetFlag;     //更新時間標誌位
unsigned char time_buf2[16];

void Init_Timer0(void);//定時器初始化
void UART_Init(void);
/*------------------------------------------------
                    主函式
------------------------------------------------*/
void main (void)
{
	unsigned char i;                  
	unsigned char temp[16];//定義顯示區域臨時儲存陣列

	LCD_Init();           //初始化液晶
	DelayMs(20);          //延時有助於穩定
	LCD_Clear();          //清屏
	Init_Timer0();        //定時器0初始化
	Ds1302_Init();        //ds1302初始化
	UART_Init();          //串列埠初始化

	Ds1302_Read_Time();   //首次讀取時間
	if((time_buf1[2]+time_buf1[7])==0) //如果所有參數都為0，寫入一個初始值
		Ds1302_Write_Time();    
	while (1)         //主循環
	{
		if(SetFlag)     //如果接收到串列埠資訊則更新時鐘
		{
			for(i=0;i<8;i++)
			{
				time_buf1[i]=time_buf2[2*i]*10+time_buf2[2*i+1];//數據整合，如2個數 1和5整合成15
			}
			Ds1302_Write_Time();//接收更新的時間然後寫入ds1302
			SetFlag=0;          //時鐘資訊更新后標誌位清零
		}


		if(ReadTimeFlag==1) //定時讀取ds1302 定時時間到 則標誌位置1，處理過時間參數標誌位清零
		{
			ReadTimeFlag=0;  //標誌位清零
			Ds1302_Read_Time();//讀取時間參數
			sprintf(temp,"DATE %02d-%02d-%02d %d",(int)time_buf1[1],(int)time_buf1[2],(int)time_buf1[3],(int)time_buf1[7]);//年月日周
			LCD_Write_String(0,0,temp);//顯示第一行
			sprintf(temp,"TIME %02d:%02d:%02d",(int)time_buf1[4],(int)time_buf1[5],(int)time_buf1[6]);//時分秒
			LCD_Write_String(0,1,temp);//顯示第二行
		}
	}
}
/*------------------------------------------------
              串列埠通訊初始化
------------------------------------------------*/
void UART_Init(void)
{
	SCON  = 0x50;		        // SCON: 模式 1, 8-bit UART, 使能接收  
	TMOD |= 0x20;               // TMOD: timer 1, mode 2, 8-bit 重灌
	TH1   = 0xFD;               // TH1:  重灌值 9600 波特率 晶振 11.0592MHz  
	TR1   = 1;                  // TR1:  timer 1 打開                         
	EA    = 1;                  //打開總中斷
	ES    = 1;                  //打開串列埠中斷
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
	if(num==50)        //大致100ms
	{
		num=0;
		ReadTimeFlag=1; //讀標誌位置1
	}
}

/*------------------------------------------------
                  串列埠中斷程式
------------------------------------------------*/
void UART_SER (void) interrupt 4 //序列中斷服務程式
{
	unsigned char Temp;          //定義臨時變數 
	unsigned char i;
	if(RI)                        //判斷是接收中斷產生
	{
		RI=0;                      //標誌位清零
		Temp=SBUF;                 //讀入緩衝區的值
		time_buf2[i]=Temp&0x0F;
		i++;
		if(i==16)                  //連續接收16個字元資訊
		{
			i=0;
			SetFlag=1;               //接收完成標誌位置1
		}
		SBUF=Temp; //把接收到的值再發回電腦端
	}
	if(TI)  //如果是發送標誌位，清零
		TI=0;
} 
