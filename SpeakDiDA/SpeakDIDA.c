/*-----------------------------------------------
  �W�١G��z
  �׾¡Gwww.doflye.net
  �s�g�Gdennis
  ����G2020.10.5
  �ק�G�L
  ���e�G�����w���n
------------------------------------------------*/
#include<reg52.h> //�]�t���Y�ɮסA�@�뱡�p���ݭn��ʡA���Y�ɮץ]�t�S��\��Ȧs�����w�q

sbit SPK=P1^2;    //�w�q��z��
unsigned char frq; 

void DelayUs2x(unsigned char t);//�禡�n�� 
void DelayMs(unsigned char t);

/*------------------------------------------------
                    �D�禡
------------------------------------------------*/
main()
{
	unsigned int i;

	while(1)
	{
		for(i=0;i<500;i++)
		{
			DelayUs2x(200); 
			DelayUs2x(200); 
			SPK=!SPK;
		}
		for(i=0;i<500;i++)
		{
			DelayMs(1);  
			SPK=!SPK;
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
