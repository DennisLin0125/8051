/*-----------------------------------------------
  名稱：單個獨立按鍵控制
  論壇：www.doflye.net
  編寫：dennis
  日期：2020.9.9
  修改：無
  內容：按鍵加減數字，多個數碼管顯示(不閃爍)
------------------------------------------------*/
#include<reg52.h> //包含標頭檔案，一般情況不需要改動，標頭檔案包含特殊功能暫存器的定義

sbit KEY_ADD=P3^3;  //定義按鍵輸入埠
sbit KEY_DEC=P3^4;

#define DataPort P0 //定義數據埠 程式中遇到DataPort 則用P0 替換
sbit LATCH1=P2^2;//定義鎖存使能埠 段鎖存
sbit LATCH2=P2^3;//                 位鎖存

unsigned char code dofly_DuanMa[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};// 顯示段碼值0~9
unsigned char code dofly_WeiMa[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//分別對應相應的數碼管點亮,即位碼
unsigned char TempData[8]; //儲存顯示值的全域性變數

void DelayUs2x(unsigned char t);//函式聲明 
void DelayMs(unsigned char t);
void Display(unsigned char FirstBit,unsigned char Num);
/*------------------------------------------------
                    主函式
------------------------------------------------*/
void main (void)
{
	unsigned char num=0;                  
	KEY_ADD=1; //按鍵輸入埠電平置高
	KEY_DEC=1;
	while (1)         //主循環
	{

		if(!KEY_ADD)  //如果檢測到低電平，說明按鍵按下
		{
			Display(0,8);  //用掃瞄函式代替延時 大約 16ms
			//DelayMs(10); //延時去抖，一般10-20ms
			if(!KEY_ADD)     //再次確認按鍵是否按下，沒有按下則退出
			{
				while(!KEY_ADD) //如果確認按下按鍵等待按鍵釋放，沒有釋放則一直執行數碼管掃瞄，防止閃爍
				{
					Display(0,8);
				}
				
				if(num<99) num++;   //加操作
				
			}
		}

		if(!KEY_DEC)  //如果檢測到低電平，說明按鍵按下
		{
			Display(0,8);  //用掃瞄函式代替延時 大約 16ms
			//DelayMs(10); //延時去抖，一般10-20ms
			if(!KEY_DEC)     //再次確認按鍵是否按下，沒有按下則退出
			{
				while(!KEY_DEC)
				{
					Display(0,8);//如果確認按下按鍵等待按鍵釋放，沒有釋放則一直執行數碼管掃瞄，防止閃爍
				}
				
				if(num>0) num--; //減操作	
			}
		}
		TempData[0]=dofly_DuanMa[num/10];//分解顯示資訊，如要顯示68，則68/10=6  68%10=8  
		TempData[1]=dofly_DuanMa[num%10];
		Display(0,8); //顯示全部8位
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
/*------------------------------------------------
 顯示函式，用於動態掃瞄數碼管
 輸入參數 FirstBit 表示需要顯示的第一位，如賦值2表示從第三個數碼管開始顯示
 如輸入0表示從第一個顯示。
 Num表示需要顯示的位數，如需要顯示99兩位數值則該值輸入2
------------------------------------------------*/
void Display(unsigned char FirstBit,unsigned char Num)
{
	unsigned char i;

	for(i=0;i<Num;i++)
	{ 
		DataPort=0;   //清空數據，防止有交替重影
		LATCH1=1;     //段鎖存
		LATCH1=0;

		DataPort=dofly_WeiMa[i+FirstBit]; //取位碼 
		LATCH2=1;     //位鎖存
		LATCH2=0;

		DataPort=TempData[i]; //取顯示數據，段碼
		LATCH1=1;     //段鎖存
		LATCH1=0;

		DelayMs(2); // 掃瞄間隙延時，時間太長會閃爍，太短會造成重影

	}

}
