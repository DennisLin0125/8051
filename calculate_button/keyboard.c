/*-----------------------------------------------
  �W�١G�x�}��L
  �s�g�Gdennis
  ����G2020.10.10
  �ק�G�L
  ���e�G
------------------------------------------------*/
#include<reg52.h> //�]�t���Y�ɮסA�@�뱡�p���ݭn��ʡA���Y�ɮץ]�t�S��\��Ȧs�����w�q
#include"keyboard.h"
#include"delay.h"

#define KeyPort P1

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
           �i�H�ھڻݭn���ܪ�^��

  			| 1 | 2 | 3 | + |  
  			| 4 | 5 | 6 | - |  
  			| 7 | 8 | 9 | * |  
  			| 0 | . | = | / | 
------------------------------------------------*/
unsigned char KeyPro(void)
{
	switch(KeyScan())
	{
		case 0x7e:return 1  ;break;//0 ���U����������ܬ۹������X��
		case 0x7d:return 2  ;break;//1
		case 0x7b:return 3  ;break;//2
		case 0x77:return '+';break;//3

		case 0xbe:return 4  ;break;//4
		case 0xbd:return 5  ;break;//5
		case 0xbb:return 6  ;break;//6
		case 0xb7:return '-';break;//7

		case 0xde:return 7  ;break;//8
		case 0xdd:return 8  ;break;//9
		case 0xdb:return 9  ;break;//a
		case 0xd7:return 'x';break;//b

		case 0xee:return 0  ;break;//c
		case 0xed:return '.';break;//d
		case 0xeb:return '=';break;//e
		case 0xe7:return '/';break;//f
		default:return 0xff;break;
	}
}