/*-----------------------------------------------
  �W�١GDS18b20 �ū��˴��G�����
  �s�g�Gdennis
  ����G2020.10.9
  �ק�G�L
  ���e�G
------------------------------------------------*/
#include<reg52.h> //�]�t���Y�ɮסA�@�뱡�p���ݭn��ʡA���Y�ɮץ]�t�S��\��Ȧs�����w�q
#include<stdio.h>
#include "18b20.h"
#include "18b20a.h"
#include "1602.h"
#include "delay.h"

bit ReadTempFlag;//�w�qŪ�ɶ��лx

void Init_Timer0(void);//�w�ɾ���l��
/*------------------------------------------------
              ��C��q�T��l��
------------------------------------------------*/
void UART_Init(void)
{
	SCON  = 0x50;		        // SCON: �Ҧ� 1, 8-bit UART, �ϯ౵��  
	TMOD |= 0x20;               // TMOD: timer 1, mode 2, 8-bit ����
	TH1   = 0xFD;               // TH1:  ����� 9600 �i�S�v ���� 11.0592MHz  
	TR1   = 1;                  // TR1:  timer 1 ���}                         
	//EA    = 1;                  //���}�`���_
	//ES    = 1;                  //���}��C���_
	TI=1;
}
/*------------------------------------------------
                    �D�禡
------------------------------------------------*/
void main (void)
{                  
	int temp;
	float temperature;
	char displaytemp[16];//�w�q��ܰϰ��{���x�s�}�C

	LCD_Init();           //��l�ƲG��
	DelayMs(20);          //���ɦ��U��í�w
	LCD_Clear();          //�M��
	Init_Timer0();
	UART_Init();
	Lcd_User_Chr();       //�g�J�۩w�q�r��

	LCD_Write_Char(13,0,0x01);//�g�J�ūץk�W���I
	LCD_Write_Char(14,0,'C'); //�g�J�r��C

	LCD_Write_Char(13,1,0x01);//�g�J�ūץk�W���I
	LCD_Write_Char(14,1,'C'); //�g�J�r��C

	while (1)         //�D�`��
	{

		if(ReadTempFlag==1)
		{
			ReadTempFlag=0;
			temp=ReadTemperature();
			temperature=(float)temp*0.0625;
			sprintf(displaytemp,"U8  %7.3f",temperature);//�C�L�ū׭�
			LCD_Write_String(0,0,displaytemp);//��ܲĤG��

			temp=ReadTemperature_a();
			temperature=(float)temp*0.0625;
			sprintf(displaytemp,"U9  %7.3f",temperature);//�C�L�ū׭�
			LCD_Write_String(0,1,displaytemp);//��ܲĤG��
		}	
	}
}

/*------------------------------------------------
                    �w�ɾ���l�Ƥl�{��
------------------------------------------------*/
void Init_Timer0(void)
{
	TMOD |= 0x01;	  //�ϥμҦ�1�A16��w�ɾ��A�ϥ�"|"�Ÿ��i�H�b�ϥΦh�өw�ɾ��ɤ����v�T		     
	//TH0=0x00;	      //���w���
	//TL0=0x00;
	EA=1;            //�`���_���}
	ET0=1;           //�w�ɾ����_���}
	TR0=1;           //�w�ɾ��}�����}
}
/*------------------------------------------------
                 �w�ɾ����_�l�{��
------------------------------------------------*/
void Timer0_isr(void) interrupt 1 
{
	static unsigned int num;
	TH0=(65536-2000)/256;		  //���s��� 2ms
	TL0=(65536-2000)%256;

	num++;
	if(num==600)        //
	{
		num=0;
		ReadTempFlag=1; //Ū�лx��m1
	}
}


