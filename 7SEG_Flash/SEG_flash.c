/*-----------------------------------------------
  �W�١G8��ƽX�ްʺA�������
  �s�g�Gdennis.lin
  ����G2020.9.7
  �ק�G�L
  ���e�G8��ƽX�ޤ��O��ܤ��P�Ʀr�A�o�ر�����ܤ覡�����ʺA����
  
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


void Delay(unsigned int t); //�禡�n��

/*------------------------------------------------
                    �D�禡
------------------------------------------------*/
main()
{
	unsigned char i=0,j;
	unsigned char Flag;
	while(1)
	{
		DataPort=dofly_WeiMa[i]; //����X 
		LATCH2=1;     //����s
		LATCH2=0;
		
		if(Flag)  //�P�_�O�_�O�{�{�лx�H
			DataPort=dofly_DuanMa[i]; //����ܼƾڡA�q�X���`���							
		else  
			DataPort=0; //�����
		
		LATCH1=1;     //�q��s
		LATCH1=0;

		Delay(200); //���˶��ة��ɡA�ɶ��Ӫ��|�{�{�A�ӵu�|�y�����v		 
		i++;
		
		if(8==i) i=0;   //�˴�8�챽�˧��������H�p���˧����h�q�Ĥ@�Ӷ}�l�A������8��
			
		j++;
		
		if(200==j)  //���ɰ{�{�ɶ�
		{
			j=0;
			Flag=!Flag;
		}
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
