/*-----------------------------------------------
  �W�١G��Ӧ@���ƽX�ްʺA����ܤƼƦr
  �s�g�Gdennis_lin	
  ����G2020.9.4
  �ק�G�L
  ���e�G�q�L�`���˴�P1�f�����J�ȡA�M��μƽX�޿�X�A
        �S��������U�ɭ�Ȥ��� 
------------------------------------------------*/
#include<reg52.h> //�]�t���Y�ɮסA�@�뱡�p���ݭn��ʡA
//���Y�ɮץ]�t�S��\��Ȧs�����w�q

unsigned char code dofly_table[10]={0xc0,0xf9,0xa4,0xb0,0x99,
                                    0x92,0x82,0xf8,0x80,0x90,}; 
									// ��ܼƭȪ�0-9

/*------------------------------------------------
                    �D�禡
------------------------------------------------*/
void main (void)
{              
	while (1)         //�D�`��
	{
		switch(P3)  //P3�f�@���W�߫����J�ݡA�˴���q���ð��p�U�P�_                            
		{
			case 0xfe:
				P1=dofly_table[1];break;//0xfe = 1111 1110,
			//�����s�u�bP3.0�𪺫���Q���U,��ܹ������Ʀr�M����X�`��
			case 0xfd:
				P1=dofly_table[2];break;//�I�s�����ĤT��
			//���� 0xa4 �U�� 0 �~��ܰ}�C�����Ĥ@�Ӥ���
			case 0xfb:
				P1=dofly_table[3];break;
			case 0xf7:
				P1=dofly_table[4];break;
			case 0xef:
				P1=dofly_table[5];break;
			case 0xdf:
				P1=dofly_table[6];break;
			case 0xbf:
				P1=dofly_table[7];break;
			case 0x7f:
				P1=dofly_table[8];break;
			default:
				break;    //�p�G���S���U�A�������X
		}
		//�D�`�����s�W��L�ݭn�@���u�@���{��
	}
}