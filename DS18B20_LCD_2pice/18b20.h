#ifndef __DS18B20B_H__
#define __DS18B20B_H__

#include<reg52.h>     //�]�t���Y�ɮסA�@�뱡�p���ݭn��ʡA���Y�ɮץ]�t�S��\��Ȧs�����w�q
#include<intrins.h>

#define uchar unsigned char
#define uint   unsigned int;
/*------------------------------------------------
                    ��w�q
------------------------------------------------*/
sbit DQ=P1^3;//ds18b20 ��
/*------------------------------------------------
                  �禡�n��
------------------------------------------------*/
unsigned int ReadTemperature(void);
bit Init_DS18B20(void);
unsigned char ReadOneChar(void);
void WriteOneChar(unsigned char dat);

#endif
