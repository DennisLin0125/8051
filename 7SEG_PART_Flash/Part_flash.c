/*-----------------------------------------------
  �W�١G�h��ƽX�ްʺA��������ܤƼƾ�
  �s�g�Gdennis.lin
  ����G2020.9.7
  �ק�G�L
  ���e�G�h��ƽX�ޤ��O��ܤ��P�Ʀr�A�o�ر�����ܤ覡�����ʺA���ˡA�ä����ܤƽ��
		  
		  �NJ50�u��(�Y��(74HC373)VCC)
		  �ñN2���T���u�qP2.2(�q��) P2.3(����)�PJ2�s��
		  �NP0�PJ3�s��
------------------------------------------------*/
#include<reg52.h> //�]�t���Y�ɮסA�@�뱡�p���ݭn��ʡA���Y�ɮץ]�t�S��\��Ȧs�����w�q

#define DataPort P0 //�w�q�ƾڰ� �{�����J��DataPort �h��P0 ����
sbit LATCH1=P2^2;//�w�q��s�ϯ�� �q��s
sbit LATCH2=P2^3;//                 ����s

unsigned char code dofly_DuanMa[10]={
	0x3f,0x06,0x5b,0x4f,0x66,
	0x6d,0x7d,0x07,0x7f,0x6f};// ��ܬq�X��0~9
unsigned char code dofly_WeiMa[]={
	0xfe,0xfd,0xfb,0xf7,
	0xef,0xdf,0xbf,0x7f};//���O�����������ƽX���I�G,�Y��X

unsigned char TempData[8]; //�x�s��ܭȪ�������ܼ�
void Delay(unsigned int t); //�禡�n��
void Display(unsigned char FirstBit,unsigned char Num);
/*------------------------------------------------
                    �D�禡
------------------------------------------------*/
main()
{
	unsigned int numA=999,numB=0;
	unsigned int j;
	bit FlashFlag;
	while(1)
	{
		j++;
		if(j==20)  //�˴��ثe�ƭ���ܤF�@�p�q�ɶ��Z�A
		  //�ݭn��ܪ��ƭȡA��{�ƾ���ܪ��ܤ�
		{
			j=0;
			FlashFlag=!FlashFlag;
			numB++;
			
			if(numA>0)//�Ω����0~999
				numA--;
			else
				numA=999;
			
			if(numB==1000) numB=0;
			
		}
		//����T��ƽX�ޡA�a���� 
		TempData[0]=dofly_DuanMa[numA/100%10];
		TempData[1]=dofly_DuanMa[numA/10%10];
		TempData[2]=dofly_DuanMa[numA%10];
		
		//����2��ƽX�ޤ���ܡA���0
		if(FlashFlag)
		{
			TempData[3]=0x40; //���"-"
			TempData[4]=0x40; 
		}
		else
		{
			TempData[3]=0; 
			TempData[4]=0; 
		}
		//���W���T��ƽX�ޡA�a����
		TempData[5]=dofly_DuanMa[numB/100%10];  
		TempData[6]=dofly_DuanMa[numB/10%10];
		TempData[7]=dofly_DuanMa[numB%10];

		Display(0,8);//�q�Ĥ@����ܡA�@���8��
	}
}
/*------------------------------------------------
 ���ɨ禡�A�t����J�Ѽ� unsigned int t�A�L��^��
 unsigned int �O�w�q�L�Ÿ�����ܼơA��Ȫ��d��O
 0~65535
------------------------------------------------*/
void Delay(unsigned int t)
{
	while(--t);
}
/*------------------------------------------------
 ��ܨ禡�A�Ω�ʺA���˼ƽX��
 ��J�Ѽ� FirstBit ��ܻݭn��ܪ��Ĥ@��A�p���2��ܱq�ĤT�ӼƽX�޶}�l���
 �p��J0��ܱq�Ĥ@����ܡC
 Num��ܻݭn��ܪ���ơA�p�ݭn���99���ƭȫh�ӭȿ�J2
------------------------------------------------*/
void Display(unsigned char FirstBit,unsigned char Num)
{
	unsigned char i;

	for(i=0;i<Num;i++)
	{ 
		DataPort=0;   //�M�żƾڡA���������v
		LATCH1=1;     //�q��s
		LATCH1=0;

		DataPort=dofly_WeiMa[i+FirstBit]; //����X 
		LATCH2=1;     //����s
		LATCH2=0;

		DataPort=TempData[i]; //����ܼƾڡA�q�X
		LATCH1=1;     //�q��s
		LATCH1=0;

		Delay(200); // ���˶��ة��ɡA�ɶ��Ӫ��|�{�{�A�ӵu�|�y�����v

	}

}