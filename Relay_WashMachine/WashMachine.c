/*-----------------------------------------------
  �W�١G�W�߫��䱱���~�q���}��
  �s�g�Gdennis
  ����G2020.10.5
  �ק�G�L
  ���e�G�������~�q�������ݥΧ����u�s�u��uln2003�~�q�������
        �q�L����2���~�q���`���u�@���A�A�����~�������������
        �@�륿��3s�A����2s�A�M�����3s�A����2s�A�`���W�z�ʧ@
------------------------------------------------*/
#include<reg52.h> //�]�t���Y�ɮסA�@�뱡�p���ݭn��ʡA���Y�ɮץ]�t�S��\��Ȧs�����w�q

#define KeyPort P3

#define DataPort P0 //�w�q�ƾڰ� �{�����J��DataPort �h��P0 ����
sbit LATCH1=P2^2;//�w�q��s�ϯ�� �q��s
sbit LATCH2=P2^3;//                 ����s

sbit RELAY1 = P1^1;//�w�q�~�q���T����X��1
sbit RELAY2 = P1^2;//�w�q�~�q���T����X��2
/*------------------------------------------------
                  ������ܼ�
------------------------------------------------*/
unsigned char hour,minute,second;//�w�q�ɤ���

bit UpdateTimeFlag;//�w�qŪ�ɶ��лx

unsigned char code dofly_DuanMa[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};// ��ܬq�X��0~9
unsigned char code dofly_WeiMa[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//���O�����������ƽX���I�G,�Y��X
unsigned char TempData[8]; //�x�s��ܭȪ�������ܼ�
/*------------------------------------------------
                  �禡�n��
------------------------------------------------*/
void DelayUs2x(unsigned char t);//us�ũ��ɨ禡�n�� 
void DelayMs(unsigned char t); //ms�ũ���
void Display(unsigned char FirstBit,unsigned char Num);//�ƽX����ܨ禡
unsigned char KeyScan(void);//��L����
void Init_Timer0(void);//�w�ɾ���l��
void Stop(void);
void Clockwise (void);
void Anticlockwise(void);
/*------------------------------------------------
                    �D�禡
------------------------------------------------*/
void main (void)
{
	unsigned char num;                  
	Init_Timer0();    //��l�Ʃw�ɾ�0�A�D�n�Ω�ƽX�ްʺA����
	Stop();           //��l�Ƭ~����������
	while (1)         //�D�`��
	{
		num=KeyScan();    //�`���I�s���䱽��

		if(num==1)        //�w�ɪ��׼W�[
		{
			if(minute<99)
				minute++;
		}
		else if(num==2)   //�w�ɪ��״�p
		{
			if(minute>0)
				minute--;
		}
		if(UpdateTimeFlag==1)//�w�ɭ��s��z��ܼƾ�
		{
			UpdateTimeFlag=0;
			TempData[2]=dofly_DuanMa[minute/10];//������ܸ�T�A�p�n���68�A�h68/10=6  68%10=8 
			TempData[3]=dofly_DuanMa[minute%10];//������ܸ�T�A�p�n���68�A�h68/10=6  68%10=8  
			TempData[4]=0x40;
			TempData[5]=dofly_DuanMa[second/10];//������ܸ�T�A�p�n���68�A�h68/10=6  68%10=8 
			TempData[6]=dofly_DuanMa[second%10];//������ܸ�T�A�p�n���68�A�h68/10=6  68%10=8 

			if((minute==0)&&(second==0))//�w�ɮɶ���A�~����������
			{
				Stop();
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
 ��J�Ѽ� FirstBit ��ܻݭn��ܪ��Ĥ@��A�p���2��
 �ܱq�ĤT�ӼƽX�޶}�l��ܡA�p��J0��ܱq�Ĥ@����ܡC
 Num��ܻݭn��ܪ���ơA�p�ݭn���99���ƭȫh�ӭ�
 ��J2
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
	static unsigned int num,i,times;
	TH0=(65536-2000)/256;		  //���s��� 2ms
	TL0=(65536-2000)%256;

	Display(0,8);                // �I�s�ƽX�ޱ���
	i++;
	if(i==20)          //20ms��s�@��
	{
		i=0;
		UpdateTimeFlag=1; //��s�ɶ��Ӧ�m1
	}
	num++;
	if(num==500)        //�j�P1s
	{
		num=0;

		///////////////////////////////////////
		//���楿����ʧ@�C
		switch(times)
		{
			case 1:Clockwise ();   break;//����3s
			case 4:Stop();         break;//����2s
			case 6:Anticlockwise();break;//����3s
			case 9:Stop();         break;//����2s
			case 11:times=0;       break;//�M�s�`���W�z�ʧ@
			default:break;
		}
		times++;
		///////////////////////////////////////////////////////
		if((minute!=0)||second)//�p�G�����M����0�A���i��p��
		second--;//���1
		if(second==0xff)//�p�G=0�Z�A��1�h���59�A�Y00�L�����59
		{
			second=59;
			if(minute>0)//�˭p�ɱ���
			{
				minute--;
			}

		}
	}
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

/*------------------------------------------------
                 ���ɰw���
------------------------------------------------*/
void Clockwise (void)
{
	RELAY1=1;    //�~�q��1�l�X�A���q���V�u��
	RELAY2=0;
}
/*------------------------------------------------
                 �f�ɰw���
------------------------------------------------*/
void Anticlockwise(void)
{
	RELAY1=0;
	RELAY2=1;   //�~�q��2�l�X�A���q�ϦV�u��
}

/*------------------------------------------------
                 �������
------------------------------------------------*/
void Stop(void)
{
	RELAY1=0;   
	RELAY2=0;
}
