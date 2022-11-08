/*-----------------------------------------------
  �W�١G�x�}��L�����K�X��
  �׾¡Gwww.doflye.net
  �s�g�Gdennis
  ����G2020.9.22
  �ק�G�L
  ���e�G�����K�X��A�K�X�T�ƨ�ROM ����q�L�����J���
------------------------------------------------*/
#include<reg52.h> //�]�t���Y�ɮסA�@�뱡�p���ݭn��ʡA���Y�ɮץ]�t�S��\��Ȧs�����w�q


#define DataPort P0 //�w�q�ƾڰ� �{�����J��DataPort �h��P0 ����
#define KeyPort P1
sbit LATCH1=P2^2;//�w�q��s�ϯ�� �q��s
sbit LATCH2=P2^3;//                 ����s

unsigned char code dofly_DuanMa[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,
		                  	         0x77,0x7c,0x39,0x5e,0x79,0x71};// ��ܬq�X��0~F
unsigned char code dofly_WeiMa[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//���O�����������ƽX���I�G,�Y��X
unsigned char TempData[10]; //�x�s��ܭȪ�������ܼ�
unsigned char code password[8]={1,2,3,4,5,6,7,8};
//�i�H��惡�K�X���h�մ���

void DelayUs2x(unsigned char t);//us�ũ��ɨ禡�n�� 
void DelayMs(unsigned char t); //ms�ũ���
void Display(unsigned char FirstBit,unsigned char Num);//�ƽX����ܨ禡
unsigned char KeyScan(void);//��L����
unsigned char KeyPro(void);
void Init_Timer0(void);//�w�ɾ���l��
/*------------------------------------------------
                    �D�禡
------------------------------------------------*/
void main (void)
{
	unsigned char num,i,j;                  
	unsigned char temp[8];
	bit Flag;
	Init_Timer0();

	while (1)         //�D�`��
	{
		num=KeyPro();
		if(num!=0xff)
		{
			if(i==0)
			{
				for(j=0;j<8;j++)//�M��
					TempData[j]=0;
			}
			if(i<8)
			{
				temp[i]=dofly_DuanMa[num];//�����ȿ�J���{�ɰ}�C��
				for(j=0;j<=i;j++)         //�q�L�@�w���ǧ��{�ɰ}�C��	
					TempData[7-i+j]=temp[j];//���Ƚ�Ȩ���ܽw�İϡA�q�k������J
			}
			i++;   //��J�ƭȲ֥[
			if(i==9)//���`����8�Y�i�A�ѩ�ڭ̻ݭn�Ť@�ӥΩ�M�̡A
			//�M�̮ɪ����䤣����J��
			{
				i=0;
				Flag=1;//��������m1
				for(j=0;j<8;j++)//�`�����8�ӼƭȡA�p�G���@�Ӥ��� �h�̲�Flag�Ȭ�0
					Flag=Flag&&(temp[j]==dofly_DuanMa[password[j]]);
				//�����J�ȩM�w���K�X
				for(j=0;j<8;j++)//�M��
					TempData[j]=0;
				if(Flag)//�p�G��������ۦP�A�лx��m1
				{
					TempData[0]=0x3f; // "o"
					TempData[1]=0x73; // "p"
					TempData[2]=0x79; // "E"
					TempData[3]=0x54; // "n"
					//�����K�X���T�A��J�����ާ@ ���"open"
				}
				else 
				{
					TempData[0]=0x79; // "E"
					TempData[1]=0x50; // "r"
					TempData[2]=0x50; // "r"
					//�_�h���"Err"
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
unsigned char KeyScan(void)  //��L���˨禡�A�ϥΦ�C���౽�˪k
{
	unsigned char cord_h,cord_l;//��C�Ȥ����ܼ�
	KeyPort=0x0f;            //��u��X����0
	cord_h=KeyPort&0x0f;     //Ū�J�C�u��
	if(cord_h!=0x0f)    //���˴����L������U
	{
		DelayMs(10);        //�h��
		if((KeyPort&0x0f)!=0x0f)
		{
			cord_h=KeyPort&0x0f;  //Ū�J�C�u��
			KeyPort=cord_h|0xf0;  //��X�ثe�C�u��
			cord_l=KeyPort&0xf0;  //Ū�J��u��

			while((KeyPort&0xf0)!=0xf0);//�����P�}�ÿ�X

			return(cord_h+cord_l);//��L�̫�զX�X��
		}
	}
	return(0xff);     //��^�ӭ�
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