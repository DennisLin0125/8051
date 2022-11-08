#include "myFunc.h"

void delayMs (uint y)
{
	uchar x=162,i=1;
	for(i=1;i<=y;i++)
	{
		while(x--);
		x=162;
	}	
}