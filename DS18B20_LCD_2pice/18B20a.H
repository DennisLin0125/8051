#ifndef __DS18B20A_H__
#define __DS18B20A_H__

#include<reg52.h>     //包含標頭檔案，一般情況不需要改動，標頭檔案包含特殊功能暫存器的定義
#include<intrins.h>

#define uchar unsigned char
#define uint   unsigned int;
/*------------------------------------------------
                    埠定義
------------------------------------------------*/
sbit DQ1=P1^4;//ds18b20 埠
/*------------------------------------------------
                  函式聲明
------------------------------------------------*/
unsigned int ReadTemperature_a(void);
bit Init_DS18B20_a(void);
unsigned char ReadOneChar_a(void);
void WriteOneChar_a(unsigned char dat);

#endif
