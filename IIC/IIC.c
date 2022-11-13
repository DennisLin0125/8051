//-----函數宣告,變數定義---------------

#include <reg52.h>
#include <intrins.h>

sbit SDA=P1^0;
sbit SCL=P1^1;

#define NUM 10            //接收和發送緩衝區的深度
#define delayNOP() {_nop_();_nop_();_nop_();_nop_();_nop_();}

#define uchar_8 unsigned char
	
uchar_8 idata sendbuf[NUM];       //資料發送緩衝區
uchar_8 idata receivebuf[NUM];    //資料接收緩衝區

bit bdata SystemError;            //slave錯誤旗標位元
//--------------------------------------------------------
// 函數名稱 : i2C_start()
// 函數功能 : 啟動i2C匯流排
//--------------------------------------------------------
void i2C_start(void)
{
	EA=0; 
	SDA=1;
	SCL=1;      //時脈保持高,資料線從高到低一次變化,I2C通訊開始      
	delayNOP(); //延遲5us
	SDA=0;
	delayNOP();
	SCL=0;
}
//--------------------------------------------------------
// 函數名稱 : i2C_stop()
// 函數功能 : 停止i2C匯流排
//--------------------------------------------------------
void i2C_stop(void)
{     
	SDA=0;      //時脈保持高,資料線從低到高一次變化,I2C通訊停止
	SCL=1;
	delayNOP(); 
	SDA=1;
	delayNOP();
	SCL=0;
}
//--------------------------------------------------------
// 函數名稱 : slave_ACK()
// 函數功能 : slave發送ACK位元
//--------------------------------------------------------
void slave_ACK(void)
{     
	SDA=0;      
	SCL=1;
	delayNOP(); 
	SDA=1;
	SCL=0;
}
//--------------------------------------------------------
// 函數名稱 : slave_NOACK()
// 函數功能 : slave發送NOACK位元,使資料傳輸過程結束
//--------------------------------------------------------
void slave_NOACK(void)
{     
	SDA=1;      
	SCL=1;
	delayNOP(); 
	SDA=0;
	SCL=0;
}
//--------------------------------------------------------
// 函數名稱 : check_ACK()
// 函數功能 : 主機ACK位元檢查
//--------------------------------------------------------
void check_ACK(void)
{     
	SDA=1;       //將P1.0設置成輸入,必須先拉高電位
	SCL=1;
	F0=0;
	if(SDA==1)   //若SDA=1顯示非應答,置位元非應答旗標F0
			F0=1;
	SCL=0;
}
//--------------------------------------------------------
// 函數名稱 : i2C_SendByte()
// 輸入參數 : ch
// 函數功能 : 發送1Byte資料
//--------------------------------------------------------
void i2C_SendByte(uchar_8 ch)
{     
	uchar_8 idata n=8;      //向SDA上發送一個位元組,共8位元
	while (n--)
	{
		if((ch&0x80)==0x80)  //若要發送的資料最高位元為1,則發送1
		{
			SDA=1;            //傳送1
			SCL=1;
			delayNOP(); 
			SDA=0;
			SCL=0;
		}
		else                 //否則傳送0
		{
			SDA=0;            //傳送0
			SCL=1;
			delayNOP(); 
			SCL=0;
		}
		ch<<=1;            
	}
}
//--------------------------------------------------------
// 函數名稱 : i2C_ReceiveByte()
// 函數功能 : 接收1Byte資料
//--------------------------------------------------------
uchar_8 i2C_ReceiveByte(void)
{     
	uchar_8 idata n=8;      //從SDA上讀取一個位元組,共8位元
	uchar_8 tdata ;
	while (n--)
	{
		SDA=1;            
		SCL=1;
		tdata<<=1;
		if(SDA==1)      
		{
			tdata|=0x01; //若接收到的位元為1,則資料的最後一位置1
		}
		else                 
		{
			tdata&=0xfe; //否則資料的最後一位置0
		}
		SCL=0;            
	}
	return(tdata);
}
//--------------------------------------------------------
// 函數名稱 : write_NByte()
// 輸入參數 : slave_add表示slave地址,n表示要發送的資料個數
// 函數功能 : 發送N位元資料
//--------------------------------------------------------
void write_NByte(uchar_8 slave_add,uchar_8 n)
{
	uchar_8 idata send_data,i=0;
	i2C_start();                     //啟動i2C
	i2C_SendByte(slave_add);         //發送地址位
	check_ACK();                     //檢查應答位元
	if(F0==1)
	{
		SystemError=1;                //若非應答,置錯誤旗標SystemError
		return;
	}
	while(n--)
	{
		send_data=sendbuf[i++];
		i2C_SendByte(send_data);
		check_ACK();                  //檢查應答位元
		if(F0==1)
		{
			SystemError=1;             //若非應答,置錯誤旗標SystemError
			return;
		}
	}
	i2C_stop();                      //全部發送後停止
}
//--------------------------------------------------------
// 函數名稱 : receive_NByte()
// 輸入參數 : slave_add表示slave地址,n表示要發送的資料個數
// 函數功能 : 接收N位元資料
//--------------------------------------------------------
void receive_NByte(uchar_8 idata slave_add,uchar_8 n)
{
	uchar_8 idata receive_data,i=0;
	i2C_start();                     //啟動i2C
	i2C_SendByte(slave_add);         //發送地址位
	check_ACK();                     //檢查應答位元
	if(F0==1)
	{
		SystemError=1;                //若非應答,置錯誤旗標SystemError
		return;
	}
	while(n--)
	{
		receive_data=i2C_ReceiveByte();
		receivebuf[i++]=receive_data;
		slave_ACK();                  //收到一個byte後,發送一個應答位元
		if(F0==1)
		{
			SystemError=1;             //若非應答,置錯誤旗標SystemError
			return;
		}
	}
	slave_NOACK();                   //收到最後一個位元組後,發送一個非應答位元
	i2C_stop();                      //全部發送後停止
}


