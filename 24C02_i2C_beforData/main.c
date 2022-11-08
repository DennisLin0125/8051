/*-----------------------------------------------
  名稱：IIC協議 24c02儲存 上次使用狀態
  編寫：dennis
  修改：無
  內容：用按鍵加減操作一個數字，數字變化后寫入24c02，當關機重新再開機，呼叫上次最後使用的數值
        這種方式用於儲存使用者的自用數據和上次使用狀態
------------------------------------------------*/  
#include <reg52.h>                
#include "i2c.h"
#include "delay.h"
#include "display.h"

sbit KEY_ADD=P3^3;  //定義按鍵輸入埠
sbit KEY_DEC=P3^4;

/*-----------------------------------------------
                     主函式
------------------------------------------------*/  
main()
{
	unsigned char num=0;
	Init_Timer0();

	IRcvStr(0xae,0,&num,1);                                

	KEY_ADD=1; //按鍵輸入埠電平置高
	KEY_DEC=1;
	while (1)         //主循環
	{

		if(!KEY_ADD)  //如果檢測到低電平，說明按鍵按下
		{
			DelayMs(10); //延時去抖，一般10-20ms
			if(!KEY_ADD)     //再次確認按鍵是否按下，沒有按下則退出
			{
				while(!KEY_ADD);//如果確認按下按鍵等待按鍵釋放，沒有釋放則一直等待
				{
					if(num<999)    //加操作
					{
						num++;
						ISendStr(0xae,0,&num,1);  //寫入24c02 
						DelayMs(10);
					}

				}
			}
		}

		if(!KEY_DEC)  //如果檢測到低電平，說明按鍵按下
		{
			DelayMs(10); //延時去抖，一般10-20ms
			if(!KEY_DEC)     //再次確認按鍵是否按下，沒有按下則退出
			{
				while(!KEY_DEC);//如果確認按下按鍵等待按鍵釋放，沒有釋放則一直等待
				{
					if(num>0)  //減操作
					{
						num--;
						ISendStr(0xae,0,&num,1);  //寫入24c02
						DelayMs(10);
					}

				}
			}
		}
		TempData[0]=dofly_DuanMa[num/100%10];    
		TempData[1]=dofly_DuanMa[num/10%10];
		TempData[2]=dofly_DuanMa[num%10];
		//主循環中新增其他需要一直工作的程式

	}
}
