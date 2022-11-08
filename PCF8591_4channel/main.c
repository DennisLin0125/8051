/*-----------------------------------------------
  �W�١GIIC��ĳ PCF8591 AD/DA�ഫ
  �׾¡Gwww.doflye.net
  �s�g�Gshifang
  �ק�G�L
  ���e�G�ϥ�4��AD����4���˴��~�������q��J
------------------------------------------------*/  
#include <reg52.h>                
#include "i2c.h"
#include "delay.h"
#include "display.h"

#define AddWr 0x90   //�g�ƾڦa�} 
#define AddRd 0x91   //Ū�ƾڦa�}

extern bit ack;
bit ReadADFlag;



unsigned char ReadADC(unsigned char Chl);
bit WriteDAC(unsigned char dat);
/*------------------------------------------------
              �D�{��
------------------------------------------------*/
main()
{
	unsigned char num=0,i;
	Init_Timer0();
	DelayMs(20);

	while (1)         //�D�`��
	{
		if(ReadADFlag)
		{
			ReadADFlag=0; 
			//�s��Ū5���A��J�q�D�Z�hŪ�X���A���̫�@���ȡA�H�KŪ�Xí�w��
			for(i=0;i<5;i++)
				num=ReadADC(0);
			// x10��ܧ��ڭ��X�j10�A�p4.5 �ܦ� 45 ��K���U�@�B�B�z x5 ��ܰ�ǹq��5V 
			num=num*5*10/256;   
			TempData[0]=dofly_DuanMa[num/10]|0x80;    
			TempData[1]=dofly_DuanMa[num%10];

			for(i=0;i<5;i++)
				num=ReadADC(1);
			num=num*5*10/256;   // x10��ܧ��ڭ��X�j10�A�p4.5 �ܦ� 45 ��K���U�@�B�B�z
			TempData[2]=dofly_DuanMa[num/10]|0x80;    
			TempData[3]=dofly_DuanMa[num%10];

			for(i=0;i<5;i++)
				num=ReadADC(2);
			num=num*5*10/256;   // x10��ܧ��ڭ��X�j10�A�p4.5 �ܦ� 45 ��K���U�@�B�B�z
			TempData[4]=dofly_DuanMa[num/10]|0x80;    
			TempData[5]=dofly_DuanMa[num%10];

			for(i=0;i<5;i++)
				num=ReadADC(3);
			num=num*5*10/256;   // x10��ܧ��ڭ��X�j10�A�p4.5 �ܦ� 45 ��K���U�@�B�B�z
			TempData[6]=dofly_DuanMa[num/10]|0x80;    
			TempData[7]=dofly_DuanMa[num%10];
			//�D�`�����s�W��L�ݭn�@���u�@���{��
		}
	}
}
/*------------------------------------------------
             ŪAD��ȵ{��
��J�Ѽ� Chl ��ܻݭn�ഫ���q�D�A�d��q0-3
��^�Ƚd��0-255
------------------------------------------------*/
unsigned char ReadADC(unsigned char Chl)
{
	unsigned char Val;
	Start_I2c();               //�Ұʶ׬y��
	SendByte(AddWr);             //�o�e����a�}
	if(ack==0)return(0);
	SendByte(Chl);            //�o�e����l�a�}
	if(ack==0)return(0);
	Start_I2c();
	SendByte(AddRd);
	if(ack==0)return(0);
	Val=RcvByte();
	NoAck_I2c();                 //�o�e�D����
	Stop_I2c();                  //�����׬y��
	return(Val);
}
/*------------------------------------------------
               �g�JDA�ഫ�ƭ�
��J�ѼơGdat ��ܻݭn�ഫ��DA�ƭȡA�d��O0-255
------------------------------------------------*/
/*bit WriteDAC(unsigned char dat)
{
   Start_I2c();               //�Ұʶ׬y��
   SendByte(AddWr);             //�o�e����a�}
     if(ack==0)return(0);
   SendByte(0x40);            //�o�e����l�a�}
     if(ack==0)return(0);
   SendByte(dat);             //�o�e�ƾ�
     if(ack==0)return(0);
   Stop_I2c();  
}*/


