#include <reg52.h>
#include "myFunc.h"
#include "delay.h"

void main (void)
{
	int i;
	uchar temp=0xfc;
	P1=0x01;
	while(1)
	{
		for(i=0;i<7;i++)
		{
			delayMs(100);
			temp<<=1;
			temp|=0x01;
			P1=temp;		
		}
		
		temp=0x3f;
		P1=temp;
		for(i=0;i<7;i++)
		{
			delayMs(100);
			temp>>=1;
			temp|=0x80;
			P1=temp;
		}
	}
}