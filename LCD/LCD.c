/*-----------------------------------------------
  �W�١GLCD1602
  �׾¡Gwww.doflye.net
  �s�g�Gdennis
  ����G2020.10.5
  �ק�G�L
  ���e�G�q�L�зǵ{���R�A��ܦr��
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
	//while(LCD_Check_Busy()); //���h����
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
		LCD_Write_Com(0x80 + x);     //��ܲĤ@��
	}
	else 
	{      
		LCD_Write_Com(0xC0 + x);      //��ܲĤG��
	}        
	while (*s) 
	{     
		LCD_Write_Data(*s);     
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
	LCD_Write_Data(Data);  
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
                    �D�禡
------------------------------------------------*/ 
void main(void) 
{ 
	LCD_Init(); 
	LCD_Clear();//�M��
	while (1) 
	{  
		LCD_Write_Char(7,0,'o');
		LCD_Write_Char(8,0,'k');
		LCD_Write_String(1,1,"Hello Dennis");
		while(1);
	}
}

