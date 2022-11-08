/*-----------------------------------------------
  �W�١G�x�}��L�̦���J���� �ϥΦ�C�v�ű��� ���_�覡
  �׾¡Gwww.doflye.net
  �s�g�Gdennis
  ����G2020.9.22
  �ק�G�L
  ���e�G�p�p�⾹��J�ƾڧΦ��ۦP �q�k�ܥ� �ϥΦ�C���ˤ�k
        ���_�覡�i�H���Ĵ���cpu�u�@�Ĳv�A�b������ʧ@�ɤ~���ˡA���ɤ��i�汽�ˤu�@
------------------------------------------------*/
#include<reg52.h> //�]�t���Y�ɮסA�@�뱡�p���ݭn��ʡA���Y�ɮץ]�t�S��\��Ȧs�����w�q


#define DataPort P0 //�w�q�ƾڰ� �{�����J��DataPort �h��P0 ����
#define KeyPort  P1

sbit LATCH1=P2^2;//�w�q��s�ϯ�� �q��s
sbit LATCH2=P2^3;//                 ����s

bit KeyPressFlag;//�w�q����лx��

unsigned char code dofly_DuanMa[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,
		                  	         0x77,0x7c,0x39,0x5e,0x79,0x71};// ��ܬq�X��0~F
unsigned char code dofly_WeiMa[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//���O�����������ƽX���I�G,�Y��X
unsigned char TempData[10]; //�x�s��ܭȪ�������ܼ�

void DelayUs2x(unsigned char t);//us�ũ��ɨ禡�n�� 
void DelayMs(unsigned char t); //ms�ũ���
void Display(unsigned char FirstBit,unsigned char Num);//�ƽX����ܨ禡
unsigned char KeyScan(void);//��L����
unsigned char KeyPro(void);
void Init_Timer0(void);//�w�ɾ���l��
void Init_INT0(void);  //�~�����_0��l�ƩM
/*------------------------------------------------
                    �D�禡
------------------------------------------------*/
void main (void)
{
	unsigned char num,i,j;                  
	unsigned char temp[8];
	Init_Timer0();
	Init_INT0();
	while (1)         //�D�`��
	{
		KeyPort=0xf0;// ��ȥΩ��_�˴�
		if(KeyPressFlag==1)
		{
			KeyPressFlag=0;  //����лx�M�s�A�H�K�U���˴�
			num=KeyPro();    
			if(num!=0xff)
			{
				if(i<8)
				{
					temp[i]=dofly_DuanMa[num];
					for(j=0;j<=i;j++)
						TempData[7-i+j]=temp[j];
				}
				i++;
				if(i==9)//�h�X�@�ӫ����J?�F�M�� �쥻���Ӭ�8
				{
					i=0;
					for(j=0;j<8;j++)//�M��
						TempData[j]=0;
				}
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
	TH0=(65536-2000)/256;		  //���s��� 2ms
	TL0=(65536-2000)%256;

	Display(0,8);       // �I�s�ƽX�ޱ���

}

/*------------------------------------------------
���䱽�˨禡�A��^�������
------------------------------------------------*/
unsigned char KeyScan(void)  //��L���˨禡�A�ϥΦ�C�v�ű��˪k
{
	unsigned char Val;
	KeyPort=0xf0;//���|��m���A�C�|��ԧC
	if(KeyPort!=0xf0)//��ܦ�������U
	{
		DelayMs(10);  //�h��
		if(KeyPort!=0xf0)
		{           //��ܦ�������U
			KeyPort=0xfe; //�˴��Ĥ@��
			if(KeyPort!=0xfe)
			{
				Val=KeyPort&0xf0;
				Val+=0x0e;
				while(KeyPort!=0xfe);
				DelayMs(10); //�h��
				while(KeyPort!=0xfe);
				return Val;
			}
			KeyPort=0xfd; //�˴��ĤG��
			if(KeyPort!=0xfd)
			{
				Val=KeyPort&0xf0;
				Val+=0x0d;
				while(KeyPort!=0xfd);
				DelayMs(10); //�h��
				while(KeyPort!=0xfd);
				return Val;
			}
			KeyPort=0xfb; //�˴��ĤT��
			if(KeyPort!=0xfb)
			{
				Val=KeyPort&0xf0;
				Val+=0x0b;
				while(KeyPort!=0xfb);
				DelayMs(10); //�h��
				while(KeyPort!=0xfb);
				return Val;
			}
			KeyPort=0xf7; //�˴��ĥ|��
			if(KeyPort!=0xf7)
			{
				Val=KeyPort&0xf0;
				Val+=0x07;
				while(KeyPort!=0xf7);
				DelayMs(10); //�h��
				while(KeyPort!=0xf7);
				return Val;
			}
		}
	}
	return 0xff;
}
/*------------------------------------------------
����ȳB�z�禡�A��^�����
------------------------------------------------*/
unsigned char KeyPro(void)
{
	switch(KeyScan())
	{
		case 0x7e:return 0;break;//0 ���U����������ܬ۹������X��
		case 0x7d:return 1;break;//1
		case 0x7b:return 2;break;//2
		case 0x77:return 3;break;//3
		case 0xbe:return 4;break;//4
		case 0xbd:return 5;break;//5
		case 0xbb:return 6;break;//6
		case 0xb7:return 7;break;//7
		case 0xde:return 8;break;//8
		case 0xdd:return 9;break;//9
		case 0xdb:return 10;break;//a
		case 0xd7:return 11;break;//b
		case 0xee:return 12;break;//c
		case 0xed:return 13;break;//d
		case 0xeb:return 14;break;//e
		case 0xe7:return 15;break;//f
		default:return 0xff;break;
	}
}
/*------------------------------------------------
           �~�����_0��l��
------------------------------------------------*/
void Init_INT0(void)
{
	EA=1;          //����ʤ��_�}
	EX0=1;         //�~�����_0�}
	IT0=1;         //��uĲ�o
}

/*------------------------------------------------
                 �~�����_0�{��
------------------------------------------------*/
void ISR_INT0(void) interrupt 0 
{
	KeyPressFlag=1;   //������䦳�ʧ@�A�i�H�i����䱽��
}
