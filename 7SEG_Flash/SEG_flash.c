/*-----------------------------------------------
  名稱：8位數碼管動態掃瞄顯示
  編寫：dennis.lin
  日期：2020.9.7
  修改：無
  內容：8位數碼管分別顯示不同數字，這種掃瞄顯示方式成為動態掃瞄
  
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


void Delay(unsigned int t); //函式聲明

/*------------------------------------------------
                    主函式
------------------------------------------------*/
main()
{
	unsigned char i=0,j;
	unsigned char Flag;
	while(1)
	{
		DataPort=dofly_WeiMa[i]; //取位碼 
		LATCH2=1;     //位鎖存
		LATCH2=0;
		
		if(Flag)  //判斷是否是閃爍標誌？
			DataPort=dofly_DuanMa[i]; //取顯示數據，段碼正常顯示							
		else  
			DataPort=0; //不顯示
		
		LATCH1=1;     //段鎖存
		LATCH1=0;

		Delay(200); //掃瞄間隙延時，時間太長會閃爍，太短會造成重影		 
		i++;
		
		if(8==i) i=0;   //檢測8位掃瞄完全結束？如掃瞄完成則從第一個開始再次掃瞄8位
			
		j++;
		
		if(200==j)  //延時閃爍時間
		{
			j=0;
			Flag=!Flag;
		}
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
