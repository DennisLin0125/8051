/*-----------------------------------------------
  名稱：多位數碼管動態掃瞄顯示變化數據
  編寫：dennis.lin
  日期：2020.9.7
  修改：無
  內容：多位數碼管分別顯示不同數字，這種掃瞄顯示方式成為動態掃瞄，並不停往左顯示
		  
		  將J50短路(即給(74HC373)VCC)
		  並將2條訊號線從P2.2(段鎖) P2.3(位鎖)與J2連接
		  將P0與J3連接
------------------------------------------------*/
#include<reg52.h> //包含標頭檔案，一般情況不需要改動，標頭檔案包含特殊功能暫存器的定義

#define DataPort P0 //定義數據埠 程式中遇到DataPort 則用P0 替換
sbit LATCH1=P2^2;//定義鎖存使能埠 段鎖存
sbit LATCH2=P2^3;//                 位鎖存

unsigned char code dofly_DuanMa[10]={
	0x3f,0x06,0x5b,0x4f,0x66,
	0x6d,0x7d,0x07,0x7f,0x6f};// 顯示段碼值0~9
unsigned char code dofly_WeiMa[]={
	0xfe,0xfd,0xfb,0xf7,
	0xef,0xdf,0xbf,0x7f};//分別對應相應的數碼管點亮,即位碼

unsigned char TempData[10]; //儲存顯示值的陣列
/*------------------------------------------------
                  函式聲明
------------------------------------------------*/
void Delay(unsigned int t); //函式聲明
void Display(unsigned char FirstBit,unsigned char Num);
/*------------------------------------------------
                    主函式
------------------------------------------------*/
main()
{

	unsigned int i,k,j;
	unsigned char s;
	while(1)
	{
		j++;
		if(j==10) //左移的速度
		{
			j=0;
			if(k==0)
			{
				for(s=0;s<10;s++) //完全循環完成後清零所有緩衝區
				TempData[s]=0;
			}
			
			TempData[8-i]=dofly_DuanMa[k]; //把需要顯示的字元依次送緩衝區
			TempData[8+1-i]=0;  //不需要顯示的區域清零
			
			if(i==(8-k))
			{
				i=0;
				k++;
				if(k==8) k=0;	
			}
			i++;
		}
		Display(0,8);//從第一位顯示，共顯示8位
	}
}
/*------------------------------------------------
 延時函式，含有輸入參數 unsigned int t，無返回值
 unsigned int 是定義無符號整形變數，其值的範圍是
 0~65535
------------------------------------------------*/
void Delay(unsigned int t)
{
	while(--t);
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

		Delay(200); // 掃瞄間隙延時，時間太長會閃爍，太短會造成重影

	}

}