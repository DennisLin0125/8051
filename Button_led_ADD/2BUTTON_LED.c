/*-----------------------------------------------
  名稱：單個獨立按鍵控制led輸出狀態
  編寫：dennis
  日期：2020.9.9
  修改：無
  內容：按下加按鍵，數值增加直到最大值，按減按鍵，數值減小，直到最小值
  
		  將J50短路(即給(74HC373)VCC)
		  並將2條訊號線從P2.2(段鎖) P2.3(位鎖)與J2連接
		  將P0與J3連接
------------------------------------------------*/
#include<reg52.h> //包含標頭檔案，一般情況不需要改動，
//標頭檔案包含特殊功能暫存器的定義

sbit KEY_ADD=P3^3;  //定義按鍵輸入埠
sbit KEY_DEC=P3^4;

#define DataPort P0 //定義數據埠 程式中遇到
//DataPort 則用P0 替換
sbit LATCH1=P2^2;//定義鎖存使能埠 段鎖存
sbit LATCH2=P2^3;//                 位鎖存

unsigned char code dofly_DuanMa[10]={
	0x3f,0x06,0x5b,0x4f,0x66,
	0x6d,0x7d,0x07,0x7f,0x6f};// 顯示段碼值0~9
unsigned char code dofly_WeiMa[]={
	0xfe,0xfd,0xfb,0xf7,
	0xef,0xdf,0xbf,0x7f};//分別對應相應的數碼管點亮,即位碼

unsigned char TempData[8]; //儲存顯示值的全域性變數
/*------------------------------------------------
                    函式聲明
------------------------------------------------*/
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
			DelayMs(10); //延時去抖，一般10-20ms
			if(!KEY_ADD)     //再次確認按鍵是否按下，沒有按下則退出
			{
				while(!KEY_ADD);//如果確認按下按鍵等待按鍵釋放，沒有釋放則一直等待
				if(num<9)  num++;  //加操作
				
			}
		}

		if(!KEY_DEC)  //如果檢測到低電平，說明按鍵按下
		{
			DelayMs(10); //延時去抖，一般10-20ms
			if(!KEY_DEC)     //再次確認按鍵是否按下，沒有按下則退出
			{
				while(!KEY_DEC);//如果確認按下按鍵等待按鍵釋放，沒有釋放則一直等待
				if(num>0) num--; //減操作
		
			}
		}
		TempData[0]=dofly_DuanMa[num%10];
		Display(3,1);
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

		DelayMs(5); // 掃瞄間隙延時，時間太長會閃爍，太短會造成重影

	}

}