/*-----------------------------------------------
  �W�١GIIC��ĳ 24c02�x�s�}������
  �s�g�Gdennis
  �ק�G�L
  ���e�G�C���}���A�}���ƭȥ[1�æs�x��24c02
------------------------------------------------*/  
#include <reg52.h>                
#include "i2c.h"
#include "delay.h"
#include "display.h"


main()
{
	unsigned char num=0;
	Init_Timer0();
	IRcvStr(0xae,50,&num,1);   //�q24c02Ū�X�ƾ�                             
	num++;                 
	ISendStr(0xae,50,&num,1);  //�g�J24c02 
	DelayMs(10);
	TempData[0]=dofly_DuanMa[num/100%10];    
	TempData[1]=dofly_DuanMa[num/10%10];
	TempData[2]=dofly_DuanMa[num%10];

	while(1)
	{
	}
}
