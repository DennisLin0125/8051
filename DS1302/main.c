/*-----------------------------------------------
  �W�١GDS1302�����ƽX�����
  �׾¡Gwww.doflye.net
  �s�g�Gdennis
  ����G2020.10.8
  �ק�G�L
  ���e�GDS1302��ɮ����ƽX����ܡA�ɶ��M���������� ���U����A�`������ 
        �ɶ��榡xx-xx-xx
		  ����榡xx-xx-xx
		  �P��榡-x-   xx
------------------------------------------------*/
#include<reg52.h> //�]�t���Y�ɮסA�@�뱡�p���ݭn��ʡA���Y�ɮץ]�t�S��\��Ȧs�����w�q
#include "ds1302.h"

#define KeyPort P3 //�w�q�����

#define DataPort P0 //�w�q�ƾڰ� �{�����J��DataPort �h��P0 ����

sbit LATCH1=P2^2;//�w�q��s�ϯ�� �q��s
sbit LATCH2=P2^3;//                 ����s

bit ReadTimeFlag;//�w�qŪ�ɶ��лx

unsigned char code dofly_DuanMa[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};// ��ܬq�X��0~9
unsigned char code dofly_WeiMa[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//���O�����������ƽX���I�G,�Y��X
unsigned char TempData[8]; //�x�s��ܭȪ�������ܼ�

void DelayUs2x(unsigned char t);//us�ũ��ɨ禡�n�� 
void DelayMs(unsigned char t); //ms�ũ���
void Display(unsigned char FirstBit,unsigned char Num);//�ƽX����ܨ禡
unsigned char KeyScan(void);//��L����
void Init_Timer0(void);//�w�ɾ���l��
/*------------------------------------------------
                    �D�禡
------------------------------------------------*/
void main (void)
{
	unsigned char num,displaynum;                  

	Init_Timer0();
	Ds1302_Init();
	Ds1302_Write_Time();
	while (1)         //�D�`��
	{
		num=KeyScan();   //���䱽��
		if(num==1)
		{
			displaynum++;
			if(displaynum==3)
				displaynum=0;
		}
		if(ReadTimeFlag==1)
		{
			ReadTimeFlag=0;
			Ds1302_Read_Time();
			if(displaynum==0) //��ܮɶ�
			{
				TempData[0]=dofly_DuanMa[time_buf1[4]/10];//��	//�ƾڪ��ഫ�A
				TempData[1]=dofly_DuanMa[time_buf1[4]%10];//�]�ڭ̱ĥμƽX��0~9�����,�N�ƾڤ��}
				TempData[2]=0x40;					    	        //�[�J"-"
				TempData[3]=dofly_DuanMa[time_buf1[5]/10];//��
				TempData[4]=dofly_DuanMa[time_buf1[5]%10];
				TempData[5]=0x40;
				TempData[6]=dofly_DuanMa[time_buf1[6]/10];//��
				TempData[7]=dofly_DuanMa[time_buf1[6]%10];	
			} 
			else if(displaynum==1)//��ܤ��
			{ 
				TempData[0]=dofly_DuanMa[time_buf1[1]/10];//�~			
				TempData[1]=dofly_DuanMa[time_buf1[1]%10];
				TempData[2]=0x40;					    	        //�[�J"-"
				TempData[3]=dofly_DuanMa[time_buf1[2]/10];//��
				TempData[4]=dofly_DuanMa[time_buf1[2]%10];
				TempData[5]=0x40;
				TempData[6]=dofly_DuanMa[time_buf1[3]/10];//��
				TempData[7]=dofly_DuanMa[time_buf1[3]%10];	
			}
			else if(displaynum==2)//��ܩP  ��
			{
				TempData[0]=0x40;				
				TempData[1]=dofly_DuanMa[time_buf1[7]%10];//�P
				TempData[2]=0x40;	//�[�J"-"
				TempData[3]=0;
				TempData[4]=0;
				TempData[5]=0;
				TempData[6]=dofly_DuanMa[time_buf1[6]/10];//��
				TempData[7]=dofly_DuanMa[time_buf1[6]%10];	
			}
		}	
	}
}
/*------------------------------------------------
 uS���ɨ禡�A�t����J�Ѽ� unsigned char t�A�L��^��
 unsigned char �O�w�q�L�Ÿ��r���ܼơA��Ȫ��d��O
 0~255 �o�̨ϥδ���12M�A��T���ɽШϥζ׽s,�j�P����
 ���צp�U T=tx2+5 uS 
------------------------------------------------*/
void DelayUs2x(unsigned char t)
{   
	while(--t);
}
/*------------------------------------------------
 mS���ɨ禡�A�t����J�Ѽ� unsigned char t�A�L��^��
 unsigned char �O�w�q�L�Ÿ��r���ܼơA��Ȫ��d��O
 0~255 �o�̨ϥδ���12M�A��T���ɽШϥζ׽s
------------------------------------------------*/
void DelayMs(unsigned char t)
{
     
	while(t--)
	{
		//�j�P����1mS
		DelayUs2x(245);
		DelayUs2x(245);
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
	if(num==50)        //�j�P100ms
	{
		num=0;
		ReadTimeFlag=1; //Ū�лx��m1
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

