#ifndef __DS18B20B_H__
#define __DS18B20B_H__

#include<reg52.h>     //包含標頭檔案，一般情況不需要改動，標頭檔案包含特殊功能暫存器的定義
#include<intrins.h>

#define uchar unsigned char
#define uint   unsigned int;
/*------------------------------------------------
                    埠定義
------------------------------------------------*/
sbit DQ=P1^3;//ds18b20 埠
/*------------------------------------------------
                  函式聲明
------------------------------------------------*/
unsigned int ReadTemperature(void);
bit Init_DS18B20(void);
unsigned char ReadOneChar(void);
void WriteOneChar(unsigned char dat);

#endif
