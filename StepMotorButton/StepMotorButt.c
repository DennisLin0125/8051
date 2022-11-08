/*-----------------------------------------------
  �W�١G�B�i�q��
  �׾¡Gwww.doflye.net
  �s�g�Gdennis
  ����G2020.10.5
  �ק�G�L
  ���e�G���{���Ω����4�ۨB�i�q���`�W�X�� 
        �ϥ�1-2���y��
        1-2�ۿE�y�\�v�W���A�B�i���״�b�A�ݰʴ��
        ���Ǧp�U a-ab-b-bc-c-cd-d-da   �S��4��8��
		  ��INT0(P3.2)�Q���U��,���F�}�l�ϦV����
 ------------------------------------------------*/

#include <reg52.h>



sbit A1=P1^0; //�w�q�B�i�q���s�u��
sbit B1=P1^1;
sbit C1=P1^2;
sbit D1=P1^3;


#define Coil_AB1 {A1=1;B1=1;C1=0;D1=0;}//AB�۳q�q�A��L���_�q
#define Coil_BC1 {A1=0;B1=1;C1=1;D1=0;}//BC�۳q�q�A��L���_�q
#define Coil_CD1 {A1=0;B1=0;C1=1;D1=1;}//CD�۳q�q�A��L���_�q
#define Coil_DA1 {A1=1;B1=0;C1=0;D1=1;}//D�۳q�q�A��L���_�q
#define Coil_A1 {A1=1;B1=0;C1=0;D1=0;}//A�۳q�q�A��L���_�q
#define Coil_B1 {A1=0;B1=1;C1=0;D1=0;}//B�۳q�q�A��L���_�q
#define Coil_C1 {A1=0;B1=0;C1=1;D1=0;}//C�۳q�q�A��L���_�q
#define Coil_D1 {A1=0;B1=0;C1=0;D1=1;}//D�۳q�q�A��L���_�q
#define Coil_OFF {A1=0;B1=0;C1=0;D1=0;}//�����_�q

unsigned char Speed;
bit Flag;
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
	unsigned int i=512;//����@�g�ɶ�

	EA=1;          //����ʤ��_�}
	EX0=1;         //�~�����_0�}
	IT0=1;         //1�����uĲ�o

	Speed=10;
	while(1)
	{
		Coil_OFF
		while((i--)&&Flag)  //���V
		{  
			Coil_A1       
			DelayMs(Speed);
			Coil_AB1                //�J��Coil_AB1  ��{A1=1;B1=1;C1=0;D1=0;}�N��
			DelayMs(Speed);         //���ܳo�ӰѼƥi�H�վ�q����t ,
									  //�Ʀr�V�p�A��t�V�j,�O�x�V�p
			Coil_B1       
			DelayMs(Speed);
			Coil_BC1
			DelayMs(Speed);
			Coil_C1       
			DelayMs(Speed);
			Coil_CD1
			DelayMs(Speed);
			Coil_D1       
			DelayMs(Speed);
			Coil_DA1
			DelayMs(Speed);
		}
		Coil_OFF
		i=512;
		while((i--)&&(!Flag))//�ϦV
		{  
			Coil_A1       
			DelayMs(Speed);
			Coil_DA1                //�J��Coil_AB1  ��{A1=1;B1=1;C1=0;D1=0;}�N��
			DelayMs(Speed);         //���ܳo�ӰѼƥi�H�վ�q����t ,
									  //�Ʀr�V�p�A��t�V�j,�O�x�V�p
			Coil_D1       
			DelayMs(Speed);
			Coil_CD1
			DelayMs(Speed);
			Coil_C1       
			DelayMs(Speed);
			Coil_BC1
			DelayMs(Speed);
			Coil_B1       
			DelayMs(Speed);
			Coil_AB1
			DelayMs(Speed);
		}
	}
}

/*------------------------------------------------
                 �~�����_�{��
------------------------------------------------*/
void ISR_INT0(void) interrupt 0 
{
 
	if(!INT0)
	{
		DelayMs(10);//�b���B�i�H�s�W�h�ݰʵ{���A�������ݰʳy�����~
		if(!INT0)
		//while(!INT1);//���ݫ�������
		{
			Flag=!Flag;   
		}
	}
}