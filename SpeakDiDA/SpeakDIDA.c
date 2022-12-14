/*-----------------------------------------------
  名稱：喇叭
  論壇：www.doflye.net
  編寫：dennis
  日期：2020.10.5
  修改：無
  內容：模擬滴答聲
------------------------------------------------*/
#include<reg52.h> //包含標頭檔案，一般情況不需要改動，標頭檔案包含特殊功能暫存器的定義

sbit SPK=P1^2;    //定義喇叭埠
unsigned char frq; 

void DelayUs2x(unsigned char t);//函式聲明 
void DelayMs(unsigned char t);

/*------------------------------------------------
                    主函式
------------------------------------------------*/
main()
{
	unsigned int i;

	while(1)
	{
		for(i=0;i<500;i++)
		{
			DelayUs2x(200); 
			DelayUs2x(200); 
			SPK=!SPK;
		}
		for(i=0;i<500;i++)
		{
			DelayMs(1);  
			SPK=!SPK;
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
