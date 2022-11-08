/*-----------------------------------------------
  �W�١G�h��ƽX�ްʺA��������ܤƼƾ�
  �s�g�Gdennis.lin
  ����G2020.9.7
  �ק�G�L
  ���e�G�h��ƽX�ޤ��O��ܤ��P�Ʀr�A�o�ر�����ܤ覡�����ʺA���ˡA�ä����������
		  
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

unsigned char TempData[10]; //�x�s��ܭȪ��}�C
/*------------------------------------------------
                  �禡�n��
------------------------------------------------*/
void Delay(unsigned int t); //�禡�n��
void Display(unsigned char FirstBit,unsigned char Num);
/*------------------------------------------------
                    �D�禡
------------------------------------------------*/
main()
{

	unsigned int i,k,j;
	unsigned char s;
	while(1)
	{
		j++;
		if(j==10) //�������t��
		{
			j=0;
			if(k==0)
			{
				for(s=0;s<10;s++) //�����`��������M�s�Ҧ��w�İ�
				TempData[s]=0;
			}
			
			TempData[8-i]=dofly_DuanMa[k]; //��ݭn��ܪ��r���̦��e�w�İ�
			TempData[8+1-i]=0;  //���ݭn��ܪ��ϰ�M�s
			
			if(i==(8-k))
			{
				i=0;
				k++;
				if(k==8) k=0;	
			}
			i++;
		}
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