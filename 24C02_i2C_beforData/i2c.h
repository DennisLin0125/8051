/*-----------------------------------------------
  名稱：IIC協議 標頭檔案
  論壇：www.doflye.net
  編寫：dennis
  修改：無
  內容：
------------------------------------------------*/  
#ifndef __I2C_H__
#define __I2C_H__  
              
#include <reg52.h>          //標頭檔案的包含
#include <intrins.h>

#define  _Nop()  _nop_()        //定義空指令

/*------------------------------------------------
                    啟動匯流排
------------------------------------------------*/
void Start_I2c();
/*------------------------------------------------
                    結束匯流排
------------------------------------------------*/
void Stop_I2c();
/*----------------------------------------------------------------
                 位元組數據傳送函式               
函式原型: void  SendByte(unsigned char c);
功能:  將數據c發送出去,可以是地址,也可以是數據,發完后等待應答,並對
     此狀態位進行操作.(不應答或非應答都使ack=0 假)     
     發送數據正常，ack=1; ack=0表示被控器無應答或損壞。
------------------------------------------------------------------*/
void  SendByte(unsigned char c);
/*----------------------------------------------------------------
                 位元組數據傳送函式               
函式原型: unsigned char  RcvByte();
功能:  用來接收從器件傳來的數據,並判斷匯流排錯誤(不發應答訊號)，
     發完后請用應答函式。  
------------------------------------------------------------------*/	
unsigned char  RcvByte();
/*----------------------------------------------------------------
                     應答子函式
原型:  void Ack_I2c(void);
----------------------------------------------------------------*/
void Ack_I2c(void);
/*----------------------------------------------------------------
                     非應答子函式
原型:  void NoAck_I2c(void);
----------------------------------------------------------------*/
void NoAck_I2c(void);
/*----------------------------------------------------------------
                    向無子地址器件發送位元組數據函式               
函式原型: bit  ISendByte(unsigned char sla,ucahr c);  
功能:     從啟動匯流排到發送地址，數據，結束匯流排的全過程,從器件地址sla.
           如果返回1表示操作成功，否則操作有誤。
注意：    使用前必須已結束匯流排。
----------------------------------------------------------------*/
bit ISendByte(unsigned char sla,unsigned char c);

/*----------------------------------------------------------------
                    向有子地址器件發送多位元組數據函式               
函式原型: bit  ISendStr(unsigned char sla,unsigned char suba,ucahr *s,unsigned char no);  
功能:     從啟動匯流排到發送地址，子地址,數據，結束匯流排的全過程,從器件
          地址sla，子地址suba，發送內容是s指向的內容，發送no個位元組。
           如果返回1表示操作成功，否則操作有誤。
注意：    使用前必須已結束匯流排。
----------------------------------------------------------------*/
bit ISendStr(unsigned char sla,unsigned char suba,unsigned char *s,unsigned char no);

/*----------------------------------------------------------------
                    向無子地址器件讀位元組數據函式               
函式原型: bit  IRcvByte(unsigned char sla,ucahr *c);  
功能:     從啟動匯流排到發送地址，讀數據，結束匯流排的全過程,從器件地
          址sla，返回值在c.
           如果返回1表示操作成功，否則操作有誤。
注意：    使用前必須已結束匯流排。
----------------------------------------------------------------*/
bit IRcvByte(unsigned char sla,unsigned char *c);
/*----------------------------------------------------------------
                    向有子地址器件讀取多位元組數據函式               
函式原型: bit  ISendStr(unsigned char sla,unsigned char suba,ucahr *s,unsigned char no);  
功能:     從啟動匯流排到發送地址，子地址,讀數據，結束匯流排的全過程,從器件
          地址sla，子地址suba，讀出的內容放入s指向的儲存區，讀no個位元組。
           如果返回1表示操作成功，否則操作有誤。
注意：    使用前必須已結束匯流排。
----------------------------------------------------------------*/
bit IRcvStr(unsigned char sla,unsigned char suba,unsigned char *s,unsigned char no);


#endif
