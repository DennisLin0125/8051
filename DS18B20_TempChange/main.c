/*-----------------------------------------------
  �W�١GDS18b20 �ū��˴��G�����
  �׾¡Gwww.doflye.net
  �s�g�Gdennis
  ����G2020.10.9
  �ק�G�L
  ���e�G
------------------------------------------------*/
#include<reg52.h> //�]�t���Y�ɮסA�@�뱡�p���ݭn��ʡA���Y�ɮץ]�t�S��\��Ȧs�����w�q
#include<stdio.h>
#include "18b20.h"
#include "1602.h"
#include "delay.h"

#define KeyPort P3 //�w�q�����

bit ReadTempFlag;//�w�qŪ�ɶ��лx

void Init_Timer0(void);//�w�ɾ���l��
unsigned char KeyScan(void);//��L����
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
	int temp,tempH=50,tempL=1;
	float temperature;
	unsigned char TempFlag=0;
	char displaytemp[16],num;//�w�q��ܰϰ��{���x�s�}�C

	LCD_Init();           //��l�ƲG��
	DelayMs(20);          //���ɦ��U��í�w
	LCD_Clear();          //�M��
	Init_Timer0();
	UART_Init();
	Lcd_User_Chr();       //�g�J�۩w�q�r��


	while (1)         //�D�`��
	{
		num=KeyScan();
		switch(num)
		{
			case 1:
				if(tempH<127)
					tempH++;
					break;
			case 2:
				if(tempH>-55)
					tempH--;
					break;
			case 3:
				if(tempL<127)
					tempL++;
					break;
			case 4:
				if(tempL>-55)
					tempL--;
					break;
			default:break;
		}

		switch(TempFlag)
		{
			case 0:    //��X�ūפW���U��
			sprintf(displaytemp,"H.%3d  L.%3d ",tempH,tempL);
			  LCD_Write_String(0,1,displaytemp);//��ܲĤG��
			break;
			case 1:LCD_Write_String(0,1,"over  tempH    ");break;
			case 2:LCD_Write_String(0,1,"under  tempL   ");break;
			default:break;
		}
		if(ReadTempFlag==1)
		{
			ReadTempFlag=0;
			temp=ReadTemperature();


			temperature=temp*0.0625;

			temp>>=4;
			if(temp>tempH)
				TempFlag=1;  //���󰪷żлx
			else if(temp<tempL)
				TempFlag=2;  //�C��C�żлx
			else
				TempFlag=0;   //���`��ܼлx



			sprintf(displaytemp,"Temp  %6.2f ",temperature);//�C�L�ū׭�
			LCD_Write_String(0,0,displaytemp);//��ܲĤ@��
			LCD_Write_Char(13,0,0x01);//�g�J�ūץk�W���I
			LCD_Write_Char(14,0,'C'); //�g�J�r��C
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
	if(num==400)        //
	{
		num=0;
		ReadTempFlag=1; //Ū�лx��m1
	}
}

/*------------------------------------------------
���䱽�˨禡�A��^�������
------------------------------------------------*/
unsigned char KeyScan(void)
{
	unsigned char keyvalue;
	if(KeyPort!=0xff)
	{
		DelayMs(10);
		if(KeyPort!=0xff)
		{
			keyvalue=KeyPort;
			while(KeyPort!=0xff);
			switch(keyvalue)
			{
				case 0xfe:return 1;break;
				case 0xfd:return 2;break;
				case 0xfb:return 3;break;
				case 0xf7:return 4;break;
				case 0xef:return 5;break;
				case 0xdf:return 6;break;
				case 0xbf:return 7;break;
				case 0x7f:return 8;break;
				default:return 0;break;
			}
		}
	}
	return 0;
}


