/*-----------------------------------------------
  �W�١GDS1302�����ƽX����� ��C���s
  �׾¡Gwww.doflye.net
  �s�g�Gdennis
  ����G2020.10.9
  �ק�G�L
  ���e�G�G����ܮɶ�
		�q�Ldofly�۱a����C�𰣿��n��A���}��C��A�i�S�v�w�]9600�A�I���s�ɶ��Y�i�A�p�G����A���U�}�o�O�_�쭫�s��s
------------------------------------------------*/
#include<reg52.h> //�]�t���Y�ɮסA�@�뱡�p���ݭn��ʡA���Y�ɮץ]�t�S��\��Ȧs�����w�q
#include <stdio.h>
#include "ds1302.h"
#include "delay.h"
#include "1602.h"

bit ReadTimeFlag;//�w�qŪ�ɶ��лx
bit SetFlag;     //��s�ɶ��лx��
unsigned char time_buf2[16];

void Init_Timer0(void);//�w�ɾ���l��
void UART_Init(void);
/*------------------------------------------------
                    �D�禡
------------------------------------------------*/
void main (void)
{
	unsigned char i;                  
	unsigned char temp[16];//�w�q��ܰϰ��{���x�s�}�C

	LCD_Init();           //��l�ƲG��
	DelayMs(20);          //���ɦ��U��í�w
	LCD_Clear();          //�M��
	Init_Timer0();        //�w�ɾ�0��l��
	Ds1302_Init();        //ds1302��l��
	UART_Init();          //��C���l��

	Ds1302_Read_Time();   //����Ū���ɶ�
	if((time_buf1[2]+time_buf1[7])==0) //�p�G�Ҧ��ѼƳ���0�A�g�J�@�Ӫ�l��
		Ds1302_Write_Time();    
	while (1)         //�D�`��
	{
		if(SetFlag)     //�p�G�������C���T�h��s����
		{
			for(i=0;i<8;i++)
			{
				time_buf1[i]=time_buf2[2*i]*10+time_buf2[2*i+1];//�ƾھ�X�A�p2�Ӽ� 1�M5��X��15
			}
			Ds1302_Write_Time();//������s���ɶ��M��g�Jds1302
			SetFlag=0;          //������T��s�Z�лx��M�s
		}


		if(ReadTimeFlag==1) //�w��Ū��ds1302 �w�ɮɶ��� �h�лx��m1�A�B�z�L�ɶ��ѼƼлx��M�s
		{
			ReadTimeFlag=0;  //�лx��M�s
			Ds1302_Read_Time();//Ū���ɶ��Ѽ�
			sprintf(temp,"DATE %02d-%02d-%02d %d",(int)time_buf1[1],(int)time_buf1[2],(int)time_buf1[3],(int)time_buf1[7]);//�~���P
			LCD_Write_String(0,0,temp);//��ܲĤ@��
			sprintf(temp,"TIME %02d:%02d:%02d",(int)time_buf1[4],(int)time_buf1[5],(int)time_buf1[6]);//�ɤ���
			LCD_Write_String(0,1,temp);//��ܲĤG��
		}
	}
}
/*------------------------------------------------
              ��C��q�T��l��
------------------------------------------------*/
void UART_Init(void)
{
	SCON  = 0x50;		        // SCON: �Ҧ� 1, 8-bit UART, �ϯ౵��  
	TMOD |= 0x20;               // TMOD: timer 1, mode 2, 8-bit ����
	TH1   = 0xFD;               // TH1:  ����� 9600 �i�S�v ���� 11.0592MHz  
	TR1   = 1;                  // TR1:  timer 1 ���}                         
	EA    = 1;                  //���}�`���_
	ES    = 1;                  //���}��C���_
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
	if(num==50)        //�j�P100ms
	{
		num=0;
		ReadTimeFlag=1; //Ū�лx��m1
	}
}

/*------------------------------------------------
                  ��C���_�{��
------------------------------------------------*/
void UART_SER (void) interrupt 4 //�ǦC���_�A�ȵ{��
{
	unsigned char Temp;          //�w�q�{���ܼ� 
	unsigned char i;
	if(RI)                        //�P�_�O�������_����
	{
		RI=0;                      //�лx��M�s
		Temp=SBUF;                 //Ū�J�w�İϪ���
		time_buf2[i]=Temp&0x0F;
		i++;
		if(i==16)                  //�s�򱵦�16�Ӧr����T
		{
			i=0;
			SetFlag=1;               //���������лx��m1
		}
		SBUF=Temp; //�Ⱶ���쪺�ȦA�o�^�q����
	}
	if(TI)  //�p�G�O�o�e�лx��A�M�s
		TI=0;
} 
