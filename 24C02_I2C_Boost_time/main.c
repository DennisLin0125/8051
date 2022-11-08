/*-----------------------------------------------
  名稱：IIC協議 24c02儲存開機次數
  編寫：dennis
  修改：無
  內容：每次開機，開機數值加1並存儲到24c02
------------------------------------------------*/  
#include <reg52.h>                
#include "i2c.h"
#include "delay.h"
#include "display.h"


main()
{
	unsigned char num=0;
	Init_Timer0();
	IRcvStr(0xae,50,&num,1);   //從24c02讀出數據                             
	num++;                 
	ISendStr(0xae,50,&num,1);  //寫入24c02 
	DelayMs(10);
	TempData[0]=dofly_DuanMa[num/100%10];    
	TempData[1]=dofly_DuanMa[num/10%10];
	TempData[2]=dofly_DuanMa[num%10];

	while(1)
	{
	}
}
