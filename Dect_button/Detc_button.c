/*-----------------------------------------------
  �W�١G��ӿW�߫���̦���J����
  �s�g�GDennis
  ����G2020.9.22
  �ק�G�L
  ���e�G�p�p�⾹��J�ƾڧΦ��ۦP �q�����k �Q�ξ�Ӱ𱽺�Ū�������
------------------------------------------------*/
#include<reg52.h> //�]�t���Y�ɮסA�@�뱡�p���ݭn��ʡA���Y�ɮץ]�t�S��\��Ȧs�����w�q

#define KeyPort P3

#define DataPort P0 //�w�q�ƾڰ� �{�����J��DataPort �h��P0 ����
sbit LATCH1=P2^2;//�w�q��s�ϯ�� �q��s
sbit LATCH2=P2^3;//                 ����s

unsigned char code dofly_DuanMa[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};// ��ܬq�X��0~9
unsigned char code dofly_WeiMa[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//���O�����������ƽX���I�G,�Y��X
unsigned char TempData[10]; //�x�s��ܭȪ�������ܼ�

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
	unsigned char num,i,j;                  

	Init_Timer0();

	while (1)         //�D�`��
	{
		num=KeyScan();
		if(num)
		{
			if(i<8) TempData[i]=dofly_DuanMa[num];
			
			i++;
			if(i==9)//�h�X�@�ӫ����J?�F�M�� �쥻���Ӭ�8
			{
				i=0;
				for(j=0;j<8;j++)//�M��
					TempData[j]=0;
			}
		}
		//Display(0,8); //��ܥ���8��
		//�D�`�����s�W��L�ݭn�@���u�@���{��
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
	
	if(i==Num) i=0;
		
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

