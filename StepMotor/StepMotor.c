/*-----------------------------------------------
  �W�١G�B�i�q��
  �׾¡Gwww.doflye.net
  �s�g�Gdennis
  ����G2020.10.5
  �ק�G�L
  ���e�G���{���Ω����4�ۨB�i�q���`�W�X�� �ϥ�1���y��

 ------------------------------------------------*/

#include <reg52.h>



sbit A1=P1^0; //�w�q�B�i�q���s�u��
sbit B1=P1^1;
sbit C1=P1^2;
sbit D1=P1^3;


#define Coil_A1 {A1=1;B1=0;C1=0;D1=0;}//A�۳q�q�A��L���_�q
#define Coil_B1 {A1=0;B1=1;C1=0;D1=0;}//B�۳q�q�A��L���_�q
#define Coil_C1 {A1=0;B1=0;C1=1;D1=0;}//C�۳q�q�A��L���_�q
#define Coil_D1 {A1=0;B1=0;C1=0;D1=1;}//D�۳q�q�A��L���_�q
#define Coil_OFF {A1=0;B1=0;C1=0;D1=0;}//�����_�q

unsigned char Speed;
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
                    �D�禡
------------------------------------------------*/
main()
{
	//unsigned int i=64*16; //��2�P����
	Speed=5; //�վ�t��
	while(1)  
	{         
		Coil_A1                 //�J��Coil_A1  ��{A1=1;B1=0;C1=0;D1=0;}�N��
		DelayMs(Speed);         //���ܳo�ӰѼƥi�H�վ�q����t ,
								      //�Ʀr�V�p�A��t�V�j,�O�x�V�p
		Coil_B1
		DelayMs(Speed);
		Coil_C1
		DelayMs(Speed);
		Coil_D1
		DelayMs(Speed);
  }
}
