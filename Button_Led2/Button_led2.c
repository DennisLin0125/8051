/*-----------------------------------------------
  名稱：單個獨立按鍵控制led輸出狀態
  編寫：dennis
  日期：2020.9.9
  修改：無
  內容：按一次按鍵，led點亮，再按一次熄滅，以此循環
------------------------------------------------*/
#include<reg52.h> //包含標頭檔案，一般情況不需要改動，標頭檔案包含特殊功能暫存器的定義

sbit KEY=P3^3;  //定義按鍵輸入埠
sbit LED=P1^2;  //定義led輸出埠


void DelayUs2x(unsigned char t);//函式聲明 
void DelayMs(unsigned char t);
/*------------------------------------------------
                    主函式
------------------------------------------------*/
void main (void)
{
                  
	KEY=1; //按鍵輸入埠電平置高
	while (1)         //主循環
	{
		if(!KEY)  //如果檢測到低電平，說明按鍵按下
		{
			DelayMs(10); //延時去抖，一般10-20ms
			if(!KEY)     //再次確認按鍵是否按下，沒有按下則退出
			{
				while(!KEY);//如果確認按下按鍵等待按鍵釋放，沒有釋放則一直等待
				LED=!LED;//釋放則執行需要的程式	
			}
		}

		//主循環中新增其他需要一直工作的程式

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
