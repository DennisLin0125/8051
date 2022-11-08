/*-----------------------------------------------
  �W�١GLCD1602
  �׾¡Gwww.doflye.net
  �s�g�Gdennis
  ����G2020.10.8
  �ק�G�L
  ���e�G
  �޸}�w�q�p�U�G1-VSS 2-VDD 3-V0 4-RS 5-R/W 6-E 7-14 DB0-DB7 15-BLA 16-BLK
------------------------------------------------*/
#include "1602.h"
#include "delay.h"

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
		LCD_Write_Com(0x80 + x);     //��ܲĤ@��
	}
	else 
	{      
		LCD_Write_Com(0xC0 + x);      //��ܲĤG��
	}        
	while (*s) 
	{     
		LCD_Write_Data( *s);     
		s ++;     
	}
}
/*------------------------------------------------
              �g�J�r���禡
------------------------------------------------*/
/* void LCD_Write_Char(unsigned char x,unsigned char y,unsigned char Data) 
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
 }*/
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
   

