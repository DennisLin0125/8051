/*-----------------------------------------------
  �W�١G�G����ܭp�⾹
  �s�g�Gdennis
  ����G2020.9.22
  �ק�G�L
  ���e�G��Ƥ����B��A�S���p�ƩҦ����k�o�X�����G�����T�A���t���B��A
------------------------------------------------*/
#include<reg52.h> //�]�t���Y�ɮסA�@�뱡�p���ݭn��ʡA���Y�ɮץ]�t�S��\��Ȧs�����w�q
#include<stdio.h>
#include"display.h"
#include"delay.h"
#include"keyboard.h"


/*------------------------------------------------
                    �D�{��
------------------------------------------------*/
main()
{
	unsigned char num,i,sign;                   
	unsigned char temp[8];        //�̤j��J8��
	bit firstflag;
	int a=0,b=0;
	unsigned char s;

	Init_Timer0();   //��l�Ʃw�ɾ�0
	while (1)         //�D�`��
	{


		num=KeyPro();  //������L
		if(num!=0xff)  //�p�G���ˬO���䦳�ĭȫh�i��B�z
		{ 
			if(i==0)    //��J�O�Ĥ@�Ӧr�����ɭԻݭn����M�šA��K�[��
			{
				for(s=0;s<8;s++) //��ȧ������w�İϲM�s�A����U����J�v�T���G
					TempData[s]=0;
			}
			if(('+'==num)|| (i==8) || ('-'==num) || ('x'==num)|| ('/'==num) || ('='==num))//��J�Ʀr�̤j��8�A��J�Ÿ���ܿ�J����
			{
				i=0;  //�p�ƾ��_��

				if(firstflag==0)  //�p�G�O��J���Ĥ@�ӼƾڡA��ȵ�a�A�ç�лx��m1�A��U�@�Ӽƾڿ�J�ɥi�H�����ȵ�b
				{
					sscanf(temp,"%d",&a);//�q�@�Ӧr���J���ܼ�
					firstflag=1;
				}
				else  
					sscanf(temp,"%d",&b);
				
				for(s=0;s<8;s++) //��ȧ������w�İϲM�s�A����U����J�v�T���G
					temp[s]=0;

				///////////////////////
				if(num!='=')      //�P�_�ثe�Ÿ���ð������B�z
					sign=num;      //�p�G���O�����O�U�лx��
				else
				{  
					firstflag=0;   //�˴����J=���A�P�_�W��Ū�J���ŦX
					switch(sign)
					{
						case '+':a=a+b;
						break;
						case '-':a=a-b;
						break;
						case 'x':a=a*b;
						break;
						case '/':a=a/b;
						break;
						default:break;
					}
					sprintf(temp,"%d",a);  //�C�L�Q�i����{�ɽw�İ�
					for(s=0;s<8;s++)       //�ѩ�C�L���OASCII�X��
					{ 
						if(temp[s]==0)      //�ҥH�ݭn�ഫ�A�p�G��0���null �ƽX�ޤW�h������ܡA�ҥH���0
							TempData[s]=0;
						else if(temp[s]==0x2d)//��ܭt���A�ƽX����ܭt�� 0x40
							TempData[s]=0x40;
						else 
							TempData[s]=dofly_DuanMa[temp[s]-'0'];//��L0-9�t���h�i��ASCII �� �Ʀr�B�z�A�p�ثe�O'3'�A��'3'-'0'=3
																		//'3'��16�i���O0x33,'0'��16�i���O0x30			     
					}
					sign=0;a=b=0;            //�Χ��Z�Ҧ��ƾڲM�s
					for(s=0;s<8;s++)
						temp[s]=0;
				}
			}

			else	if(i<16)
			{
				temp[i]=num+'0'; 
				TempData[i]=dofly_DuanMa[num];//��X�ƾ�
				i++;   //��J�ƭȲ֥[
			}

		}	
	}
}

