/*-----------------------------------------------
  �W�١G�B�i�q��
  �׾¡Gwww.doflye.net
  �s�g�Gdennis
  ����G2020.10.5
  �ק�G�L
  ���e�G���{���Ω����4�ۨB�i�q���`�W�X�� 2�ӫ��� �ո`�t�� 
        �ϥ�1-2���y��
		  �ƽX����� 1-5�t�׵��šA�Ʀr�V�j�A�t�׶V�p
		  �NP3���ܿW�߫���
 ------------------------------------------------*/

#include <reg52.h>

#define KeyPort P3
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

unsigned char Speed=1;
bit StopFlag;
void Display(unsigned char FirstBit,unsigned char Num);
void Init_Timer0(void);
unsigned char KeyScan(void);
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
	unsigned char num;
	Init_Timer0();
	Coil_OFF
	while(1)  //���V
	{ 
		num=KeyScan();    //�`���I�s���䱽��
		if(num==1)//�Ĥ@�ӫ���,�t�׵��żW�[
		{
			if(Speed<5) Speed++;	
		}	
		else if(num==2)//�ĤG�ӫ���A�t�׵��Ŵ�p
		{
			if(Speed>1) Speed--;
		}	

		TempData[1]=dofly_DuanMa[Speed%10];
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
	static unsigned char times,i;
	TH0=(65536-1000)/256;		  //���s��� 1ms
	TL0=(65536-1000)%256;

	Display(0,8);

	if(times==Speed)
	{
		times=0;
		switch(i)
		{
			case 0:Coil_A1;i++;break;
			case 1:Coil_AB1;i++;break;
			case 2:Coil_B1;i++;break;
			case 3:Coil_BC1;i++;break;
			case 4:Coil_C1;i++;break;
			case 5:Coil_CD1;i++;break;
			case 6:Coil_D1;i++;break;
			case 7:Coil_DA1;i++;break;
			case 8:i=0;break;
			default:break;
		}
	}
	else
		times++;
 
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

