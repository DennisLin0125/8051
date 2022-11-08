/*-----------------------------------------------
  �W�١GDS18b20�ƽX����� 
  �׾¡Gwww.doflye.net
  �s�g�Gdennis
  ����G2020.10.10
  �ק�G�L
  ���e�G��ܮ榡 �Ÿ� xxx.x C 
        �i�H��ܭt�ū�
        �ƽX����ܦP�ɳq�L��C��o�e��q�������
        �ϥ�dofly�t�M�����ųn�� �����ϥ�11.0592M
------------------------------------------------*/
#include<reg52.h> //�]�t���Y�ɮסA�@�뱡�p���ݭn��ʡA���Y�ɮץ]�t�S��\��Ȧs�����w�q
#include<stdio.h>
#include "18b20.h"

#define DataPort P0 //�w�q�ƾڰ� �{�����J��DataPort �h��P0 ����

sbit LATCH1=P2^2;//�w�q��s�ϯ�� �q��s
sbit LATCH2=P2^3;//                 ����s

bit ReadTempFlag;//�w�qŪ�ɶ��лx

unsigned char code dofly_DuanMa[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};// ��ܬq�X��0~9
unsigned char code dofly_WeiMa[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//���O�����������ƽX���I�G,�Y��X
unsigned char TempData[8]; //�x�s��ܭȪ�������ܼ�


void Display(unsigned char FirstBit,unsigned char Num);//�ƽX����ܨ禡
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
	TI =1;
}
/*------------------------------------------------
                    �D�禡
------------------------------------------------*/
void main (void)
{                  
	unsigned int TempH,TempL,temp;
	Init_Timer0();
	UART_Init();
	while (1)         //�D�`��
	{
		if(ReadTempFlag==1)
		{
			ReadTempFlag=0;
			temp=ReadTemperature();
			if(temp&0x8000)
			{
				TempData[0]=0x40;//�t���лx
				temp=~temp;  // ���ϥ[1
				temp +=1;
			}
			else
				TempData[0]=0;

			TempH=temp>>4;
			TempL=temp&0x0F;
			TempL=TempL*6/10;//�p�ƪ���B�z
			printf("%d.%d\n",TempH,TempL);
			
			if(TempH/100==0)
				TempData[1]=0;
			else
				TempData[1]=dofly_DuanMa[TempH/100]; //�Q��ū�
			
			if((TempH/100==0)&&((TempH%100)/10==0))//����
				TempData[2]=0;
			else
				TempData[2]=dofly_DuanMa[(TempH%100)/10]; //�Q��ū�
			
			TempData[3]=dofly_DuanMa[(TempH%100)%10]|0x80; //�Ӧ�ū�,�a�p���I
			TempData[4]=dofly_DuanMa[TempL];
			TempData[6]=0x39;         //���C�Ÿ�
		}	
	}
}

/*------------------------------------------------
 ��ܨ禡�A�Ω�ʺA���˼ƽX��
 ��J�Ѽ� FirstBit ��ܻݭn��ܪ��Ĥ@��A�p���2��ܱq�ĤT�ӼƽX�޶}�l���
 �p��J0��ܱq�Ĥ@����ܡC
 Num��ܻݭn��ܪ���ơA�p�ݭn���99���ƭȫh�ӭȿ�J2
------------------------------------------------*/
void Display(unsigned char FirstBit,unsigned char Num)
{
	static unsigned char i=0;


	DataPort=0;   //�M�żƾڡA���������v
	LATCH1=1;     //�q��s
	LATCH1=0;

	DataPort=dofly_WeiMa[i+FirstBit]; //����X 
	LATCH2=1;     //����s
	LATCH2=0;

	DataPort=TempData[i]; //����ܼƾڡA�q�X
	LATCH1=1;     //�q��s
	LATCH1=0;

	i++;
	if(i==Num)
		i=0;


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

	Display(0,8);       // �I�s�ƽX�ޱ���
	num++;
	if(num==300)        //
	{
		num=0;
		ReadTempFlag=1; //Ū�лx��m1
	}
}


