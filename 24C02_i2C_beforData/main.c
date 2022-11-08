/*-----------------------------------------------
  �W�١GIIC��ĳ 24c02�x�s �W���ϥΪ��A
  �s�g�Gdennis
  �ק�G�L
  ���e�G�Ϋ���[��ާ@�@�ӼƦr�A�Ʀr�ܤƦZ�g�J24c02�A���������s�A�}���A�I�s�W���̫�ϥΪ��ƭ�
        �o�ؤ覡�Ω��x�s�ϥΪ̪��ۥμƾکM�W���ϥΪ��A
------------------------------------------------*/  
#include <reg52.h>                
#include "i2c.h"
#include "delay.h"
#include "display.h"

sbit KEY_ADD=P3^3;  //�w�q�����J��
sbit KEY_DEC=P3^4;

/*-----------------------------------------------
                     �D�禡
------------------------------------------------*/  
main()
{
	unsigned char num=0;
	Init_Timer0();

	IRcvStr(0xae,0,&num,1);                                

	KEY_ADD=1; //�����J��q���m��
	KEY_DEC=1;
	while (1)         //�D�`��
	{

		if(!KEY_ADD)  //�p�G�˴���C�q���A����������U
		{
			DelayMs(10); //���ɥh�ݡA�@��10-20ms
			if(!KEY_ADD)     //�A���T�{����O�_���U�A�S�����U�h�h�X
			{
				while(!KEY_ADD);//�p�G�T�{���U���䵥�ݫ�������A�S������h�@������
				{
					if(num<999)    //�[�ާ@
					{
						num++;
						ISendStr(0xae,0,&num,1);  //�g�J24c02 
						DelayMs(10);
					}

				}
			}
		}

		if(!KEY_DEC)  //�p�G�˴���C�q���A����������U
		{
			DelayMs(10); //���ɥh�ݡA�@��10-20ms
			if(!KEY_DEC)     //�A���T�{����O�_���U�A�S�����U�h�h�X
			{
				while(!KEY_DEC);//�p�G�T�{���U���䵥�ݫ�������A�S������h�@������
				{
					if(num>0)  //��ާ@
					{
						num--;
						ISendStr(0xae,0,&num,1);  //�g�J24c02
						DelayMs(10);
					}

				}
			}
		}
		TempData[0]=dofly_DuanMa[num/100%10];    
		TempData[1]=dofly_DuanMa[num/10%10];
		TempData[2]=dofly_DuanMa[num%10];
		//�D�`�����s�W��L�ݭn�@���u�@���{��

	}
}
