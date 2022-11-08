/*-----------------------------------------------
  �W�١GIIC��ĳ PCF8591 AD/DA�ഫ
  �׾¡Gwww.doflye.net
  �s�g�Gdennis
  �ק�G�L
  ���e�G�ϥ�4��AD����1���˴��~�������q��J
------------------------------------------------*/  
#include <reg52.h>                
#include "i2c.h"
#include "delay.h"
#include "display.h"

#define AddWr 0x90   //�g�ƾڦa�}   0b 1001 0000
#define AddRd 0x91   //Ū�ƾڦa�}   0b 1001 0001

extern bit ack;
unsigned char ReadADC(unsigned char Chl);
bit WriteDAC(unsigned char dat);
/*------------------------------------------------
              �D�{��
------------------------------------------------*/
main()
{
	unsigned char num=0;
	Init_Timer0();

	while (1)         //�D�`��
	{
		num=ReadADC(1);
		TempData[0]=dofly_DuanMa[num/100%10];    
		TempData[1]=dofly_DuanMa[num/10%10];
		TempData[2]=dofly_DuanMa[num%10];
		//�D�`�����s�W��L�ݭn�@���u�@���{��
		DelayMs(100);
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
	if(ack==0)
		return(0);
	SendByte(0x40|Chl);            //�o�e����l�a�}
	if(ack==0)
		return(0);
	Start_I2c();
	SendByte(AddWr+1);
	if(ack==0)
		return(0);
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
