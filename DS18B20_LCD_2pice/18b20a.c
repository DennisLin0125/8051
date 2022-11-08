/*-----------------------------------------------
  �W�١G18B20�ū׷P����
  �s�g�Gdennis
  ����G2020.10.9
  �ק�G�L
  ���e�G18B20��u�ū��˴������μ˨ҵ{��
------------------------------------------------*/
#include"delay.h"
#include"18b20a.h"
/*------------------------------------------------
                    18b20��l��
------------------------------------------------*/
bit Init_DS18B20_a(void)
{
	bit dat=0;
	DQ1 = 1;    //DQ1�_��
	DelayUs2x(5);   //�y������
	DQ1 = 0;         //�L����NDQ1�ԧC
	DelayUs2x(200); //��T���� �j�� 480us �p��960us
	DelayUs2x(200);
	DQ1 = 1;        //�԰��׬y��
	DelayUs2x(50); //15~60us �Z ����60-240us���s�b�߽�
	dat=DQ1;        //�p�Gx=0�h��l�Ʀ��\, x=1�h��l�ƥ���
	DelayUs2x(25); //�y�@���ɪ�^
	return dat;
}

/*------------------------------------------------
                    Ū���@�Ӧ줸��
------------------------------------------------*/
unsigned char ReadOneChar_a(void)
{
	unsigned char i=0;
	unsigned char dat = 0;
	for (i=8;i>0;i--)
	{
		DQ1 = 0; // ���߽İT��
		dat>>=1;
		DQ1 = 1; // ���߽İT��
		if(DQ1)
			dat|=0x80;
		DelayUs2x(25);
	}
	return(dat);
}
/*------------------------------------------------
                    �g�J�@�Ӧ줸��
------------------------------------------------*/
void WriteOneChar_a(unsigned char dat)
{
	unsigned char i=0;
	for (i=8; i>0; i--)
	{
		DQ1 = 0;
		DQ1 = dat&0x01;
		DelayUs2x(25);
		DQ1 = 1;
		dat>>=1;
	}
	DelayUs2x(25);
}

/*------------------------------------------------
                    Ū���ū�
------------------------------------------------*/
unsigned int ReadTemperature_a(void)
{
	unsigned char a=0;
	unsigned int b=0;
	unsigned int t=0;
	Init_DS18B20_a();
	WriteOneChar_a(0xCC); // ���LŪ�Ǹ��C�����ާ@
	WriteOneChar_a(0x44); // �Ұʷū��ഫ
	DelayMs(10);
	Init_DS18B20_a();
	WriteOneChar_a(0xCC); //���LŪ�Ǹ��C�����ާ@ 
	WriteOneChar_a(0xBE); //Ū���ū׼Ȧs�����]�@�iŪ9�ӼȦs���^ �e��ӴN�O�ū�
	a=ReadOneChar_a();   //�C��
	b=ReadOneChar_a();   //����

	b<<=8;
	t=a+b;

	return(t);
}
