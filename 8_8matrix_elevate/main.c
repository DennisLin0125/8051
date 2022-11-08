/*-----------------------------------------------
  �W�١G�����I�}
  �s�g�Gdennis
  �ק�G�L
  ���e�G�ϥ�hc595�X�������I�} 
------------------------------------------------*/
#include<reg52.h>    //�]�t���Y�ɮסA�@�뱡�p���ݭn��ʡA���Y�ɮץ]�t�S��\��Ȧs�����w�q
#include <intrins.h> 

//unsigned char  segout[8]={0,1,2,3,4,5,6,7}; //8�C
unsigned char  segout[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80}; //8�C
unsigned char code tab[]={
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x08,0x1C,0x3E,0x7F,0x1C,0x1C,0x1C,0x1C,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
/*------------------------------------------------
                �w���w�q
------------------------------------------------*/

sbit LATCH = P1^0; 
sbit SRCLK= P1^1;
sbit SER  = P1^2;


sbit LATCH_B = P2^2;
sbit SRCLK_B= P2^1;
sbit SER_B= P2^0;

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
                �o�e�줸�յ{��
------------------------------------------------*/
void SendByte(unsigned char dat)
{    
	unsigned char i; 

	for(i=0;i<8;i++)
	{
		SRCLK=0;
		SER=dat&0x80;
		dat<<=1;
		SRCLK=1;
	}
         
}
/*------------------------------------------------
                �o�e���줸�յ{��
     595���p�An��595�A�N�ݭn�o�en�줸�զZ��s
------------------------------------------------*/
void Send2Byte(unsigned char dat1,unsigned char dat2)
{    
   SendByte(dat1);
   SendByte(dat2);      
}
/*------------------------------------------------
                   595��s�{��
		  595���p�o�e�ƾڦZ�A��s����
------------------------------------------------*/
void Out595(void)
{
	LATCH=0;
	_nop_();
	LATCH=1;
}

/*------------------------------------------------
                �o�e��X�줸�յ{��
               �ϥΥt�~�@����W595
------------------------------------------------*/
void SendSeg(unsigned char dat)
{    
	unsigned char i; 

	for(i=0;i<8;i++)  //�o�e�줸��
	{
		SRCLK_B=0;
		SER_B=dat&0x80;
		dat<<=1;
		SRCLK_B=1;
	}
	LATCH_B=0;    //��s
	_nop_();
	LATCH_B=1;
         
         
}
/*------------------------------------------------
                   �D�{��
------------------------------------------------*/
void main()
{
	unsigned char i,j,k;
	while(1)
	{
		//�W�����
		for(j=0;j<16;j++)
			for(k=0;k<10;k++)
				for(i=0;i<8;i++) //8�C���
				{
					SendSeg(segout[7-i]);

					Send2Byte(~tab[i+j],0xff);
					Out595();
					DelayMs(1);

					Send2Byte(0xff,0xff);//delay(10); //����v
					Out595();
				}
		//�U�����
		for(j=0;j<16;j++)
			for(k=0;k<10;k++)
				for(i=0;i<8;i++) //8�C���
				{
					SendSeg(segout[i]);

					Send2Byte(~tab[i+j],0xff);
					Out595();
					DelayMs(1);

					Send2Byte(0xff,0xff);//delay(10); //����v
					Out595();
				}

	}
}