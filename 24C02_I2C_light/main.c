/*-----------------------------------------------
  名稱：IIC協議 24c02儲存花樣燈程式
  論壇：www.doflye.net
  編寫：dennis
  修改：無
  內容：將表格內容寫入24c02，然後批量刪除這些數據，
        並從24c02中讀取數據並顯示
------------------------------------------------*/  
#include <reg52.h>                
#include "i2c.h"
/*------------------------------------------------
                    花樣燈表格
------------------------------------------------*/
unsigned char      dat[]={0x7f,0xbf,0xdf,0xef,
						  0xf7,0xfb,0xfd,0xfe,
						  0xff,0xff,0x00,0x00,
						  0x55,0x55,0xaa,0xaa
						  };//ram允許情況可以無限新增
/*------------------------------------------------
                   主函式
------------------------------------------------*/
main()
{
	//bit flag;
	unsigned char i;
	ISendStr(0xae,80,dat,16);            //寫入24c02
	DelayMs(1);                          //寫入后必須延時等待24c02內部
													 //燒錄數據處理完畢方可繼續其他操作
	for(i=0;i<16;i++)                    //清除數據
		dat[i]=0;
	IRcvStr(0xae,80,dat,16);             //從24c02讀出數據 
	while(1)
	{

		for(i=0;i<16;i++)              //查表
		{
			DelayMs(200);
			P1=dat[i];
		}
	}
}
