/*-----------------------------------------------
  名稱：IIC協議 
  論壇：www.doflye.net
  編寫：dennis
  修改：無
  內容：函式是採用軟體延時的方法產生SCL脈衝,固對高晶振頻率要作 一定的修改....(本例是1us機器
		週期,即晶振頻率要小於12MHZ)
------------------------------------------------*/  
  
              
#include "i2c.h"
#include "delay.h"

#define  _Nop()  _nop_()  //定義空指令
                         
bit ack;	              //應答標誌位

sbit SDA=P2^1;
sbit SCL=P2^0;

/*------------------------------------------------
                    啟動匯流排
------------------------------------------------*/
void Start_I2c()
{
	SDA=1;   //發送起始條件的數據訊號
	_Nop();
	SCL=1;
	_Nop();    //起始條件建立時間大於4.7us,延時
	_Nop();
	_Nop();
	_Nop();
	_Nop();    
	SDA=0;     //發送起始訊號
	_Nop();    //起始條件鎖定時間大於4μ
	_Nop();
	_Nop();
	_Nop();
	_Nop();       
	SCL=0;    //鉗住I2C匯流排，準備發送或接收數據
	_Nop();
	_Nop();
}
/*------------------------------------------------
                    結束匯流排
------------------------------------------------*/
void Stop_I2c()
{
	SDA=0;    //發送結束條件的數據訊號
	_Nop();   //發送結束條件的時鐘訊號
	SCL=1;    //結束條件建立時間大於4μ
	_Nop();
	_Nop();
	_Nop();
	_Nop();
	_Nop();
	SDA=1;    //發送I2C匯流排結束訊號
	_Nop();
	_Nop();
	_Nop();
	_Nop();
}




/*----------------------------------------------------------------
                 位元組數據傳送函式               
函式原型: void  SendByte(unsigned char c);
功能:  將數據c發送出去,可以是地址,也可以是數據,發完后等待應答,並對
     此狀態位進行操作.(不應答或非應答都使ack=0 假)     
     發送數據正常，ack=1; ack=0表示被控器無應答或損壞。
------------------------------------------------------------------*/
void  SendByte(unsigned char c)
{
	unsigned char BitCnt;

	for(BitCnt=0;BitCnt<8;BitCnt++)  //要傳送的數據長度為8位
	{
		if((c<<BitCnt)&0x80)
			SDA=1;   //判斷發送位
		else  
			SDA=0;                
		_Nop();
		SCL=1;               //置時鐘線為高，通知被控器開始接收數據位
		_Nop(); 
		_Nop();             //保證時鐘高電平週期大於4μ
		_Nop();
		_Nop();
		_Nop();         
		SCL=0; 
	}

	_Nop();
	_Nop();
	SDA=1;               //8位發送完后釋放數據線，準備接收應答位
	_Nop();
	_Nop();   
	SCL=1;
	_Nop();
	_Nop();
	_Nop();
	if(SDA==1)
		ack=0;     
	else 
		ack=1;        //判斷是否接收到應答訊號
	
	SCL=0;
	_Nop();
	_Nop();
}







/*----------------------------------------------------------------
                 位元組數據傳送函式               
函式原型: unsigned char  RcvByte();
功能:  用來接收從器件傳來的數據,並判斷匯流排錯誤(不發應答訊號)，
     發完后請用應答函式。  
------------------------------------------------------------------*/	
unsigned char  RcvByte()
{
	unsigned char retc;
	unsigned char BitCnt;

	retc=0; 
	SDA=1;             //置數據線為輸入方式
	for(BitCnt=0;BitCnt<8;BitCnt++)
	{
		_Nop();           
		SCL=0;       //置時鐘線為低，準備接收數據位
		_Nop();
		_Nop();      //時鐘低電平週期大於4.7us
		_Nop();
		_Nop();
		_Nop();
		SCL=1;       //置時鐘線為高使數據線上數據有效
		_Nop();
		_Nop();
		retc=retc<<1;
		if(SDA==1)
			retc=retc+1; //讀數據位,接收的數據位放入retc中
		_Nop();
		_Nop(); 
	}
	SCL=0;    
	_Nop();
	_Nop();
	return(retc);
}



/*----------------------------------------------------------------
                     應答子函式
原型:  void Ack_I2c(void);
 
----------------------------------------------------------------*/
/*void Ack_I2c(void)
{
  
  SDA=0;     
  _Nop();
  _Nop();
  _Nop();      
  SCL=1;
  _Nop();
  _Nop();              //時鐘低電平週期大於4μ
  _Nop();
  _Nop();
  _Nop();  
  SCL=0;               //清時鐘線，鉗住I2C匯流排以便繼續接收
  _Nop();
  _Nop();    
}*/
/*----------------------------------------------------------------
                     非應答子函式
原型:  void NoAck_I2c(void);
 
----------------------------------------------------------------*/
void NoAck_I2c(void)
{
  
	SDA=1;
	_Nop();
	_Nop();
	_Nop();      
	SCL=1;
	_Nop();
	_Nop();              //時鐘低電平週期大於4μ
	_Nop();
	_Nop();
	_Nop();  
	SCL=0;                //清時鐘線，鉗住I2C匯流排以便繼續接收
	_Nop();
	_Nop();    
}






/*----------------------------------------------------------------
                    向無子地址器件發送位元組數據函式               
函式原型: bit  ISendByte(unsigned char sla,ucahr c);  
功能:     從啟動匯流排到發送地址，數據，結束匯流排的全過程,從器件地址sla.
           如果返回1表示操作成功，否則操作有誤。
注意：    使用前必須已結束匯流排。
----------------------------------------------------------------*/
/*bit ISendByte(unsigned char sla,unsigned char c)
{
   Start_I2c();               //啟動匯流排
   SendByte(sla);             //發送器件地址
     if(ack==0)return(0);
   SendByte(c);               //發送數據
     if(ack==0)return(0);
  Stop_I2c();                 //結束匯流排
  return(1);
}
*/

/*----------------------------------------------------------------
                    向有子地址器件發送多位元組數據函式               
函式原型: bit  ISendStr(unsigned char sla,unsigned char suba,ucahr *s,unsigned char no);  
功能:     從啟動匯流排到發送地址，子地址,數據，結束匯流排的全過程,從器件
          地址sla，子地址suba，發送內容是s指向的內容，發送no個位元組。
           如果返回1表示操作成功，否則操作有誤。
注意：    使用前必須已結束匯流排。
----------------------------------------------------------------*/
/*bit ISendStr(unsigned char sla,unsigned char suba,unsigned char *s,unsigned char no)
{
   unsigned char i;
 for(i=0;i<no;i++)
    { 
   Start_I2c();               //啟動匯流排
   SendByte(sla);             //發送器件地址
     if(ack==0)return(0);
   SendByte(suba);            //發送器件子地址
     if(ack==0)return(0); 
 
     SendByte(*s);            //發送數據
       if(ack==0)return(0);
     Stop_I2c();                  //結束匯流排
	 DelayMs(1);               //必須延時等待晶片內部自動處理數據完畢
	 s++;
	 suba++;
    } 
  return(1);
}
*/
/*----------------------------------------------------------------
                    向無子地址器件讀位元組數據函式               
函式原型: bit  IRcvByte(unsigned char sla,ucahr *c);  
功能:     從啟動匯流排到發送地址，讀數據，結束匯流排的全過程,從器件地
          址sla，返回值在c.
           如果返回1表示操作成功，否則操作有誤。
注意：    使用前必須已結束匯流排。
----------------------------------------------------------------*/
/*bit IRcvByte(unsigned char sla,unsigned char *c)
{
   Start_I2c();                //啟動匯流排
   SendByte(sla+1);            //發送器件地址
     if(ack==0)return(0);
   *c=RcvByte();               //讀取數據
     NoAck_I2c();              //發送非就答位
     Stop_I2c();               //結束匯流排
  return(1);
}

*/
/*----------------------------------------------------------------
                    向有子地址器件讀取多位元組數據函式               
函式原型: bit  ISendStr(unsigned char sla,unsigned char suba,ucahr *s,unsigned char no);  
功能:     從啟動匯流排到發送地址，子地址,讀數據，結束匯流排的全過程,從器件
          地址sla，子地址suba，讀出的內容放入s指向的儲存區，讀no個位元組。
           如果返回1表示操作成功，否則操作有誤。
注意：    使用前必須已結束匯流排。
----------------------------------------------------------------*/
/*bit IRcvStr(unsigned char sla,unsigned char suba,unsigned char *s,unsigned char no)
{
   unsigned char i;

   Start_I2c();               //啟動匯流排
   SendByte(sla);             //發送器件地址
     if(ack==0)return(0);
   SendByte(suba);            //發送器件子地址
     if(ack==0)return(0);

   Start_I2c();
   SendByte(sla+1);
      if(ack==0)return(0);

  for(i=0;i<no-1;i++)
    { 
     *s=RcvByte();              //發送數據
      Ack_I2c();                //發送就答位 
     s++;
    } 
   *s=RcvByte();
    NoAck_I2c();                 //發送非應位
    Stop_I2c();                    //結束匯流排
  return(1);
}
*/
