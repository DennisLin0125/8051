/*-----------------------------------------------
  �W�١G�h��ƽX�ްʺA��������ܤƼƾ�
  �׾¡Gwww.doflye.net
  �s�g�Gdennis.lin
  ����G2020.9.5
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
	unsigned int num=9999;
	unsigned int j,k,x;
	while(1)
	{
		j++;
		if(j==10)  //�˴��ثe�ƭ���ܤF�@�p�q�ɶ��Z�A�ݭn��ܪ��ƭ� -1�A��{�ƾ���ܪ��ܤ�
		{
			j=0;
			if(num>0)//�Ω����0~9999
				num--;
			else
				num=9999;

		}

		/*TempData[0]=dofly_DuanMa[num/1000%10];//���d���   
		TempData[1]=dofly_DuanMa[num/100%10]; //���ʦ��  
		TempData[2]=dofly_DuanMa[num/10%10];  //���Q���
		TempData[3]=dofly_DuanMa[num%10];     //���Ӧ��*/
		
		//���C�Ӧ�ƪ��j��g�k
		x=1000;
		for (k=0;k<4;k++)
		{
			TempData[k]=dofly_DuanMa[num/x%10];
			x/=10;
		}
		Display(4,4);
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