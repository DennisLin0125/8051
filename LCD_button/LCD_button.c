/*-----------------------------------------------
  �W�١GLCD1602
  �׾¡Gwww.doflye.net
  �s�g�Gdennis
  ����G2020.10.8
  �ק�G�L
  ���e�G�q�L�x�}��L��J�A�̦����0-F16���r��
  �޸}�w�q�p�U�G1-VSS 2-VDD 3-V0 4-RS 5-R/W 6-E 7-14 DB0-DB7 15-BLA 16-BLK
------------------------------------------------*/
#include<reg52.h> //�]�t���Y�ɮסA�@�뱡�p���ݭn��ʡA���Y�ɮץ]�t�S��\��Ȧs�����w�q
#include<intrins.h>

sbit RS = P2^4;   //�w�q�� 
sbit RW = P2^5;
sbit EN = P2^6;

#define RS_CLR RS=0 
#define RS_SET RS=1

#define RW_CLR RW=0 
#define RW_SET RW=1 

#define EN_CLR EN=0
#define EN_SET EN=1

#define DataPort P0
#define KeyPort P1

unsigned char code dofly_code[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};//�ഫ���G����ܪ��r��
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
              �P���禡
------------------------------------------------*/
bit LCD_Check_Busy(void) 
{ 
	DataPort= 0xFF; 
	RS_CLR; 
	RW_SET; 
	EN_CLR; 
	_nop_(); 
	EN_SET;
	return (bit)(DataPort & 0x80);
}
/*------------------------------------------------
              �g�J�R�O�禡
------------------------------------------------*/
void LCD_Write_Com(unsigned char com) 
{  
	// while(LCD_Check_Busy()); //���h����
	DelayMs(5);
	RS_CLR; 
	RW_CLR; 
	EN_SET; 
	DataPort= com; 
	_nop_(); 
	EN_CLR;
}
/*------------------------------------------------
              �g�J�ƾڨ禡
------------------------------------------------*/
void LCD_Write_Data(unsigned char Data) 
{ 
	//while(LCD_Check_Busy()); //���h����
	DelayMs(5);
	RS_SET; 
	RW_CLR; 
	EN_SET; 
	DataPort= Data; 
	_nop_();
	EN_CLR;
}


/*------------------------------------------------
                �M�̨禡
------------------------------------------------*/
void LCD_Clear(void) 
{ 
	LCD_Write_Com(0x01); 
	DelayMs(5);
}
/*------------------------------------------------
              �g�J�r��禡
------------------------------------------------*/
void LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s) 
{     
	if (y == 0) 
	{     
		LCD_Write_Com(0x80 + x);     
	}
	else 
	{     
		LCD_Write_Com(0xC0 + x);     
	}        
	while (*s) 
	{     
		LCD_Write_Data( *s);     
		s++;     
	}
}
/*------------------------------------------------
              �g�J�r���禡
------------------------------------------------*/
void LCD_Write_Char(unsigned char x,unsigned char y,unsigned char Data) 
{     
	if (y == 0) 
	{     
		LCD_Write_Com(0x80 + x);     
	}    
	else 
	{     
		LCD_Write_Com(0xC0 + x);     
	}        
	LCD_Write_Data( Data);  
}
/*------------------------------------------------
              ��l�ƨ禡
------------------------------------------------*/
void LCD_Init(void) 
{
	LCD_Write_Com(0x38);    /*��ܼҦ��]�w*/ 
	DelayMs(5); 
	LCD_Write_Com(0x38); 
	DelayMs(5); 
	LCD_Write_Com(0x38); 
	DelayMs(5); 
	LCD_Write_Com(0x38);  
	LCD_Write_Com(0x08);    /*�������*/ 
	LCD_Write_Com(0x01);    /*��ܲM��*/ 
	LCD_Write_Com(0x06);    /*��ܴ�в��ʳ]�w*/ 
	DelayMs(5); 
	LCD_Write_Com(0x0C);    /*��ܶ}�δ�г]�w*/
}
 
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
------------------------------------------------*/
unsigned char KeyPro(void)
{
	switch(KeyScan())
	{
		case 0x7e:return 0;break;//0 ���U����������ܬ۹������X��
		case 0x7d:return 1;break;//1
		case 0x7b:return 2;break;//2
		case 0x77:return 3;break;//3
		case 0xbe:return 4;break;//4
		case 0xbd:return 5;break;//5
		case 0xbb:return 6;break;//6
		case 0xb7:return 7;break;//7
		case 0xde:return 8;break;//8
		case 0xdd:return 9;break;//9
		case 0xdb:return 10;break;//a
		case 0xd7:return 11;break;//b
		case 0xee:return 12;break;//c
		case 0xed:return 13;break;//d
		case 0xeb:return 14;break;//e
		case 0xe7:return 15;break;//f
		default:return 0xff;break;
	}
}

/*------------------------------------------------
                    �D�禡
------------------------------------------------*/ 
void main(void) 
{ 
	unsigned char i,j,num; 

	LCD_Init(); 
	LCD_Write_Com(0x0F);//��ж}�A��а{�{�}

	LCD_Write_String(0,0,"Press the key !");

	while (1) 
	{  
		num=KeyPro();
		if(num!=0xff)
		{
			if((i==0)&&(j==0))//�^��Ĥ@�Ӧr���ɲM��
			LCD_Clear();//�M��
			LCD_Write_Char(0+i,0+j,dofly_code[num]);//�̦���ܿ�J�r��
			i++;
			if(i==16)//�p�G�Ĥ@����ܺ��A���ĤG��
			{
				i=0;j++;
				if(j==2) j=0;//�p�G2�泣��ܺ��A�M�̫᭫�s�q�Ĥ@�����
			}
		}
	}
}

