/*-----------------------------------------------
  �W�١G��ӿW�߫��䱱��
  �׾¡Gwww.doflye.net
  �s�g�Gdennis
  ����G2020.9.9
  �ק�G�L
  ���e�G����[��Ʀr�A�h�ӼƽX����ܡA�ϥΩw�ɾ����ƽX�ްʺA���� �ðϧO�����u���ĪG�A�����i�H���Ϊ���ڥͲ���
------------------------------------------------*/
#include<reg52.h> //�]�t���Y�ɮסA�@�뱡�p���ݭn��ʡA���Y�ɮץ]�t�S��\��Ȧs�����w�q

sbit KEY_ADD=P3^3;  //�w�q�����J��
sbit KEY_DEC=P3^4;

#define DataPort P0 //�w�q�ƾڰ� �{�����J��DataPort �h��P0 ����
sbit LATCH1=P2^2;//�w�q��s�ϯ�� �q��s
sbit LATCH2=P2^3;//                 ����s

unsigned char code dofly_DuanMa[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};// ��ܬq�X��0~9
unsigned char code dofly_WeiMa[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//���O�����������ƽX���I�G,�Y��X
unsigned char TempData[8]; //�x�s��ܭȪ�������ܼ�

void DelayUs2x(unsigned char t);//�禡�n�� 
void DelayMs(unsigned char t);
void Display(unsigned char FirstBit,unsigned char Num);
void Init_Timer0(void);
/*------------------------------------------------
                    �D�禡
------------------------------------------------*/
void main (void)
{
	unsigned char num=0,key_press_num;                
	KEY_ADD=1; //�����J��q���m��
	KEY_DEC=1;
	Init_Timer0();

	while (1)         //�D�`��
	{

		if(!KEY_ADD)  //�p�G�˴���C�q���A����������U
		{
			DelayMs(10); //���ɥh�ݡA�@��10-20ms
			if(!KEY_ADD)     //�A���T�{����O�_���U�A�S�����U�h�h�X
			{
				while(!KEY_ADD)
				{
					key_press_num++;
					DelayMs(10);           //10x200=2000ms=2s 
					if(key_press_num==200) //�j��2s
					{
						key_press_num=0;    //�p�G�F�������зǫh�i�J������ʧ@
						
						while(!KEY_ADD) //�o�̥Ω��ѧO�O�_�����٦b���U�A�p�G���U��������ʧ@�A�_�h�h�X 
						{
							if(num<99)   num++; //�[�ާ@
							  
							//�Y�ɧ���ܼƾڳB�z�A�p�G�h���U��2
							 //�y�B�z��T�A��ڤW�ݤ��캥�ܮĪG�A
							 //�ӬO�ݨ���ܮĪG
							//�ϥΪ̥i�H�ۦ�B������
							//������ܸ�T�A�p�n���68�A�h68/10=6  68%10=8
							TempData[0]=dofly_DuanMa[num/10];  
							TempData[1]=dofly_DuanMa[num%10];
							DelayMs(50);//�Ω�ո`�����`���ާ@
							//���t�סA�i�H�ۦ�վ㦹�ȥH�K�F��̨ήĪG
						}
					}
				}
				
				key_press_num=0;//����֥[�y�����~�ѧO
				if(num<99)  num++;  //�[�ާ@
				
			}
		}

		if(!KEY_DEC)  //�p�G�˴���C�q���A����������U
		{
			DelayMs(10); //���ɥh�ݡA�@��10-20ms
			if(!KEY_DEC) //�A���T�{����O�_���U�A�S�����U�h�h�X	
			{
				while(!KEY_DEC)
				{
					key_press_num++;
					DelayMs(10);
					if(key_press_num==200) //�j��2s
					{
						key_press_num=0;
						while(!KEY_DEC)
						{
							if(num>0)  num--; //��ާ@
						
							TempData[0]=dofly_DuanMa[num/10];  
							TempData[1]=dofly_DuanMa[num%10];
							
							DelayMs(50);//�Ω�ո`�����`���ާ@���t��
						}
					}
				}
				
				key_press_num=0;//����֥[�y�����~�ѧO
				if(num>0)  num--; //��ާ@
			}
		}
		//������ܸ�T�A�p�n���68�A�h68/10=6  68%10=8  
		TempData[0]=dofly_DuanMa[num/10];
		TempData[1]=dofly_DuanMa[num%10];
		// Display(0,8); //��ܥ���8��
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

	Display(0,8);

}