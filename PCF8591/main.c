/*-----------------------------------------------
  名稱：IIC協議 PCF8591 AD/DA轉換
  論壇：www.doflye.net
  編寫：dennis
  修改：無
  內容：使用4路AD中的1路檢測外部模擬量輸入
------------------------------------------------*/  
#include <reg52.h>                
#include "i2c.h"
#include "delay.h"
#include "display.h"

#define AddWr 0x90   //寫數據地址   0b 1001 0000
#define AddRd 0x91   //讀數據地址   0b 1001 0001

extern bit ack;
unsigned char ReadADC(unsigned char Chl);
bit WriteDAC(unsigned char dat);
/*------------------------------------------------
              主程式
------------------------------------------------*/
main()
{
	unsigned char num=0;
	Init_Timer0();

	while (1)         //主循環
	{
		num=ReadADC(1);
		TempData[0]=dofly_DuanMa[num/100%10];    
		TempData[1]=dofly_DuanMa[num/10%10];
		TempData[2]=dofly_DuanMa[num%10];
		//主循環中新增其他需要一直工作的程式
		DelayMs(100);
	}
}
/*------------------------------------------------
             讀AD轉值程式
輸入參數 Chl 表示需要轉換的通道，範圍從0-3
返回值範圍0-255
------------------------------------------------*/
unsigned char ReadADC(unsigned char Chl)
{
	unsigned char Val;
	Start_I2c();               //啟動匯流排
	SendByte(AddWr);             //發送器件地址
	if(ack==0)
		return(0);
	SendByte(0x40|Chl);            //發送器件子地址
	if(ack==0)
		return(0);
	Start_I2c();
	SendByte(AddWr+1);
	if(ack==0)
		return(0);
	Val=RcvByte();
	NoAck_I2c();                 //發送非應位
	Stop_I2c();                  //結束匯流排
	return(Val);
 }
/*------------------------------------------------
               寫入DA轉換數值
輸入參數：dat 表示需要轉換的DA數值，範圍是0-255
------------------------------------------------*/
/*bit WriteDAC(unsigned char dat)
{
   Start_I2c();               //啟動匯流排
   SendByte(AddWr);             //發送器件地址
     if(ack==0)return(0);
   SendByte(0x40);            //發送器件子地址
     if(ack==0)return(0);
   SendByte(dat);             //發送數據
     if(ack==0)return(0);
   Stop_I2c();  
}*/
