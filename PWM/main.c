#include <reg52.h>
#include "myFunc.h"
#include "delay.h"

sbit LED0=P1^0;

void Delay(unsigned int t);

void main (void)
{
	uint CYCLE=600,PWM_LOW=0;
	while (1) 
	{
		LED0=1;
		Delay(60000);
		for(PWM_LOW=1;PWM_LOW<CYCLE;PWM_LOW++)
		{
			LED0=0;
			Delay(PWM_LOW);
			LED0=1; 
			Delay(CYCLE-PWM_LOW);
		}
		LED0=0;
		for(PWM_LOW=CYCLE-1;PWM_LOW>0;PWM_LOW--)
		{
			LED0=0;
			Delay(PWM_LOW);
			LED0=1; 
			Delay(CYCLE-PWM_LOW);
		}
	}
}
void Delay(unsigned int t)
{
	while(--t);
}