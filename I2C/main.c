#include <reg52.h>
#include "I2C.h"
unsigned char code dofly_table[10]={
	0xc0,0xf9,0xa4,0xb0,0x99,
   0x92,0x82,0xf8,0x80,0x90,}; 
	// 顯示數值表0-9

void main()
{
	uchar address=23,mydata;
	init();
	//WriteDataToAddress(address,0);
	//delay1(100);
	mydata=RandomReadFromAddress(address);
	P0=dofly_table[mydata];
	if(mydata<9)
	{
		mydata++;
	}
	else
	{
		mydata=0;
	}
	delay1(100);
	WriteDataToAddress(address,mydata);
	while(1);
}