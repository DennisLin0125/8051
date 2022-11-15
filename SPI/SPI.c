//-----------變數定義--------------------------------------

#include <reg52.h>
#include <intrins.h>

sbit SCK = P1^0;      //定義時脈輸出
sbit MOSI = P1^1;     //定義主機輸出
sbit MISO = P1^2;     //定義主機輸入
sbit CS = P1^3;       //定義晶片選擇

#define delayNOP() {_nop_();_nop_();_nop_();_nop_();_nop_();}
#define uint_8 unsigned char
//-----------------------------------------------------------
// 函數名稱 : SPISendByte
// 輸入參數 : ch
// 函數功能 : 發送一個位元組
//-----------------------------------------------------------
void SPI_SendByte (uint_8 ch)
{
	uint_8 idata n=8;     //向MOSI線發送取一個位元組,共8位元
	
	SCK=1;                //時脈置高
	CS=0;                 //選擇slave
	
	while (n--)
	{
		delayNOP();        
		SCK=0;             //時脈置低
		if((ch&0x80)==0x80)//若發送的資料最高位元為1
		{
			MOSI=1;         //則傳送1
		}
		else
		{
			MOSI=0;         //否則傳送0
		}
		delayNOP();
		ch<<=1;            //資料左移1位元
		SCK=1;             //時脈置高
	}
}
//-----------------------------------------------------------
// 函數名稱 : SPI_ReceiveByte
// 函數功能 : 接收一個位元組
//-----------------------------------------------------------
uint_8 SPI_ReceiveByte(void)
{
	uint_8 idata n=8;     //向MISO線讀取取一個位元組,共8位元
	uint_8 tdata;
	
	SCK=1;                //時脈置高
	CS=0;                 //選擇slave
	
	while(n--)
	{
		delayNOP();
		SCK=0;             //時脈置低
		delayNOP();
		tdata<<=1;         //左移1位元
		
		if(MISO==1)        //若接收到的資料為1
		{
			tdata|=0x01;    //則tdata的最後一位元置1
		}
		else
		{
			tdata&=0xFE;    //否則tdata的最後一位元置0
		}
		SCK=1;
	}
	return(tdata);
}
//-----------------------------------------------------------
// 函數名稱 : SPI_Send_ReceiveByte
// 輸入參數 : ch
// 返回接收資料
// 函數功能 : 串列輸入/輸出副程式
//-----------------------------------------------------------
uint_8 SPI_Send_ReceiveByte(uint_8 ch)
{
	uint_8 idata n=8;     //向MISO線讀取取一個位元組,共8位元
	uint_8 tdata;
	
	SCK=1;                //時脈置高
	CS=0;                 //選擇slave
	
	while(n--)
	{
		delayNOP();
		SCK=0;             //時脈置低
		delayNOP();
		
		tdata<<=1;         //左移1位元

		if(MISO==1)        //若接收到的資料為1
		{
			tdata|=0x01;    //則tdata的最後一位元置1
		}
		else
		{
			tdata&=0xFE;    //否則tdata的最後一位元置0
		}


		if((ch&0x80)==0x80)//若發送的資料最高位元為1
		{
			MOSI=1;         //則傳送1
		}
		else
		{
			MOSI=0;         //否則傳送0
		}
		ch<<=1;            //資料左移1位元
		
		SCK=1;
	}
	return(tdata);
}
