/*-----------------------------------------------
  �W�١G�ݪ������X���� ���䤣������
  �׾¡Gwww.doflye.net
  �s�g�Gdennis
  ����G2020.10.5
  �ק�G�L
  ���e�G�q�L������������X�_�� �ݪ����t�ܵ{��  
        �b16383�Ӿ����g���������ܤ������@��
        �з�AT89s52�L�������q�L�C
------------------------------------------------*/
#include <reg52.h>

sfr WDTRST = 0xA6;

sbit  K1 = P3^0; 
sbit  K2 = P3^1;
sbit  LED1=P1^1;
sbit  LED2=P1^2;

void DelayUs2x(unsigned char t);//us�ũ��ɨ禡�n�� 
void DelayMs(unsigned char t); //ms�ũ���
/*------------------------------------------------
                    �D�禡
------------------------------------------------*/
main()
{
	LED1=0;
	DelayMs(100);
	LED1=1;
	DelayMs(100);

	TMOD=0x01;
	TH0=0xc6;      //�w��16ms
	TL0=0x66;
	EA=1;
	ET0=1;

	WDTRST=0x1e;   //�b�{����l�Ƥ��ҥάݪ����C
	WDTRST=0xe1;   //���e1E,��eE1

	if(K1==0)
	{
		TR0=1;
	}

	while(1)
	{    
		if(K2==0)
		{
			TR0=0;
		}
		LED2=1;
		LED1=1;
		DelayMs(100);
		LED2=0;
		DelayMs(100);
	}
}
/*------------------------------------------------
              �w�ɾ����_�禡
------------------------------------------------*/
void Time0(void) interrupt 1
{
	TH0=0xc6;      //�w��16ms
	TL0=0x66;

	WDTRST=0x1e;   //�������O
	WDTRST=0xe1;
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
