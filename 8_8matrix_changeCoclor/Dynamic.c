/*-----------------------------------------------
  �W�١G�����I�}
  �s�g�Gdennis
  �ק�G�L
  ���e�G�ϥ�hc595�X�������I�}
------------------------------------------------*/
#include<reg52.h>    //�]�t���Y�ɮסA�@�뱡�p���ݭn��ʡA���Y�ɮץ]�t�S��\��Ȧs�����w�q
#include <intrins.h> 


unsigned char  segout[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80}; //8�C
unsigned char code tab[96]={
                        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //�Ů�
                        0x00,0x7E,0xFF,0xC3,0xC3,0xFF,0x7E,0x00, //�r��0
                        0x00,0x00,0x43,0xFF,0xFF,0x03,0x00,0x00, //�r��1
                        0x00,0x63,0xC7,0xCF,0xDB,0xF3,0x63,0x00, //�r��2
                        0x00,0x42,0xDB,0xDB,0xDB,0xFF,0x66,0x00, //�r��3
                        0x00,0x3E,0x46,0xFF,0xFF,0x06,0x06,0x00, //�r��4
                        0x00,0xF6,0xF7,0xD3,0xD3,0xDF,0xDE,0x00, //�r��5
                        0x00,0x7E,0xFF,0xDB,0xDB,0xDF,0x4E,0x00, //�r��6
                        0x00,0xC0,0xC0,0xC7,0xFF,0xF8,0xC0,0x00, //�r��7
                        0x00,0xFF,0xFF,0xDB,0xDB,0xFF,0xFF,0x00, //�r��8
                        0x00,0x72,0xFB,0xDB,0xDB,0xFF,0x7E,0x00, //�r��9
                        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00   //�Ů�
                        };
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
	   �a����V�ѼơA�i�H��ܱq����`�g�J�Ϊ̧C��g�J
------------------------------------------------*/
void SendByte(unsigned char dat,bit direction)
{    
	unsigned char i,temp; 
	if(direction==0)
		temp=0x80;
	else
		temp=0x01;

	for(i=0;i<8;i++)
	{
		SRCLK=0;
		SER=dat&temp;
		
		if(direction==0)
			dat<<=1;
		else
			dat>>=1;
		
		SRCLK=1;
	}
         
}
/*------------------------------------------------
                �o�e���줸�յ{��
------------------------------------------------*/
void Send2Byte(unsigned char dat1,unsigned char dat2,bit direction)
{    
   SendByte(dat1,direction);
   SendByte(dat2,direction);      
}
/*------------------------------------------------
                   595��s�{��
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
	unsigned char i,k,l,speed=10;
	while(1)
	{
		for(k=0;k<=88;k++)     //�Ҧ����r���Ӽ�
			for(l=speed;l>0;l--)    //���ɪ���,���ܦ��ȥi�H���ܬy�ʳt��
				for(i=0;i<=7;i++) //8�C���
				{
					SendSeg(segout[i]);
					Send2Byte(~(*(tab+i+k)),0xff,0);
					Out595();
					DelayMs(1);
					Send2Byte(0xff,0xff,0);//delay(10); //����v
					Out595();
				}


		for(k=0;k<=88;k++)     //�Ҧ����r���Ӽ�
			for(l=speed;l>0;l--)    //���ɪ���,���ܦ��ȥi�H���ܬy�ʳt��
				for(i=0;i<=7;i++) //8�C���
				{
					SendSeg(segout[7-i]);
					Send2Byte(0xff,~(*(tab+i+k)),1); //�g�J�줸�դ�V����
					Out595();
					DelayMs(1);
					Send2Byte(0xff,0xff,0);//delay(10); //����v
					Out595();
				}
	}
}