//---------------變數定義--------------------
#include <reg52.h>
#define uint_16 unsigned int
#define uchar_8 unsigned char

sbit DQ=P1^0;      //定義時脈輸出
//-------------------------------------------
// 函數功能 : delay
// 輸入參數 : N
// 函數功能 : 時間延遲副程式,實現(16*N+24)us
// 系統採用11.0592MHz
//-------------------------------------------
void delay (uint_16 N)
{
	uint_16 i;
	for(i=0;i<N;i++);
}
//-------------------------------------------
// 函數功能 : Reset
// 輸入參數 : 無
// 返回deceive_ready
// 函數功能 : 重置
//-------------------------------------------
uchar_8 Reset(void)
{
	uchar_8 deceive_ready;
	DQ=0;                   //拉低DQ線
	delay(29);              //時間延遲至少480us~960uS
	DQ=1;                   //拉高DQ線
	delay(3);               //時間延遲,等待 deceive_ready響應
	deceive_ready=DQ;       //取樣deceive_ready訊號
	delay(25);              //等待時序結束
	return(deceive_ready);  //有deceive_ready訊號返回0,否則返回1
}
//-------------------------------------------
// 函數功能 : read_bit
// 輸入參數 : 無
// 返回接收資料
// 函數功能 : 讀1bit副程式
//-------------------------------------------
uchar_8 read_bit(void)
{
	uchar_8 i;
	DQ=0;                   //拉低DQ線,開始時序
	DQ=1;                   //拉高DQ線
	for(i=0;i<3;i++);       //時間延遲至時序開始15us
	return(DQ);             //返回DQ
}
//-------------------------------------------
// 函數功能 : write_bit
// 輸入參數 : bitval
// 函數功能 : 寫1bit副程式
//-------------------------------------------
void write_bit(uchar_8 bitval)
{
	DQ=0;                   //拉低DQ線,開始時序
	if(bitval==1)
		DQ=1;                //如果寫邏輯為1
	delay(5);           
	DQ=1;                   //拉高DQ線
}
//-------------------------------------------
// 函數功能 : write_Byte
// 輸入參數 : val
// 函數功能 : 寫1Byte副程式
//-------------------------------------------
void write_Byte(uchar_8 val)
{
	uchar_8 i,temp;
	for(i=0;i<8;i++)
	{
		temp=val>>i;            //將val位元右移i位元賦值給temp
		temp&=0x01;             //取temp最低位元
		write_bit(temp);              
		delay(5);           
	}
}
//-------------------------------------------
// 函數功能 : read_Byte
// 返回接收的資料value
// 函數功能 : 讀1Byte副程式
//-------------------------------------------
uchar_8 read_Byte(void)
{
	uchar_8 i,receive_data;
	receive_data=0;
	
	for(i=0;i<8;i++)
	{
		if(read_bit())
		{
			receive_data+=(1<<i);  //每讀一位元資料receive_data就左移一位元
		}
		delay(6); 
	}
	return(receive_data);
}


