/*-----------------------------------------------
  名稱：多位數碼管動態掃瞄顯示變化數據 數碼管消隱
  編寫：dennis.lin
  日期：2020.9.5
  修改：無
  內容：多位數碼管分別顯示不同數字，這種掃瞄顯示方式成為動態掃瞄，並不停變化賦值
        當高位值為0是不顯示該位，即消隱，如數字 0010，實際顯示為10，前2位0不顯示
		  
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

unsigned char TempData[8]; //儲存顯示值的全域性變數
void Delay(unsigned int t); //函式聲明
void Display(unsigned char FirstBit,unsigned char Num);
/*------------------------------------------------
                    主函式
------------------------------------------------*/
main()
{
	unsigned int num;
	unsigned int j;
	while(1)
	{
		j++;
		if(j==10)  //j為調整顯示的速度 
		{
			j=0;
			num++;
			if(num==10000) num=0;//用於顯示0~9999
		}
		
		if(num<1000)    //如果小於1000則千位不顯示
			TempData[0]=0;
		else
			TempData[0]=dofly_DuanMa[num/1000%10];//取千位數 
		
		if(num<100)    //如果小於100則千位和百位都不顯示
			TempData[1]=0;
		else
			TempData[1]=dofly_DuanMa[num/100%10]; //取百位數 
		
		if(num<10)   //如果小於10，則千位、百位和十位都不顯示
			TempData[2]=0;
		else
			TempData[2]=dofly_DuanMa[num/10%10];  //取十位數
		
		TempData[3]=dofly_DuanMa[num%10];     //取個位數
		Display(4,4);
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
