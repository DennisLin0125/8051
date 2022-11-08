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
		  �ƽX����� 03-19�t�׵��šA�Ʀr�V�j�A�t�׶V�C
 ------------------------------------------------*/

#include <reg52.h>

#define DataPort P0 //�w�q�ƾڰ� �{�����J��DataPort �h��P0 ����
sbit LATCH1=P2^2;//�w�q��s�ϯ�� �q��s
sbit LATCH2=P2^3;//                 ����s

unsigned char code dofly_DuanMa[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};// ��ܬq�X��0~9
unsigned char code dofly_WeiMa[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//���O�����������ƽX���I�G,�Y��X
unsigned char TempData[8]; //�x�s��ܭȪ�������ܼ�

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

void Display(unsigned char FirstBit,unsigned char Num);
void Init_Timer0(void);
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

	Init_Timer0();

	EA=1;          //����ʤ��_�}
	EX0=1;         //�~�����_0�}
	IT0=1;         //1�����uĲ�o

	Speed=3;
	TempData[0]=dofly_DuanMa[Speed/10];//������ܸ�T�A�p�n���68�A 
	TempData[1]=dofly_DuanMa[Speed%10];//�h68/10=6  68%10=8 
	Coil_OFF
	while(i--)  //���V
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
			Speed++;   
			if(Speed==20) Speed=3;
				
			TempData[0]=dofly_DuanMa[Speed/10];//������ܸ�T�A�p�n���68�A  
			TempData[1]=dofly_DuanMa[Speed%10];//�h68/10=6  68%10=8
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
	PT0=1;           //�u�����ǥ��}
}
/*------------------------------------------------
                 �w�ɾ����_�l�{��
------------------------------------------------*/
void Timer0_isr(void) interrupt 1 
{
	TH0=(65536-2000)/256;		  //���s��� 2ms
	TL0=(65536-2000)%256;

	Display(0,8);

}

