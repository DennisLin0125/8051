/*-----------------------------------------------
  �W�١GLCD1602
  �s�g�Gdennis
  ����G2020.10.9
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
�]�w�G�Ӧ۩w�q�r��,LCD1602���۩w�q�r�����a�}��0x00--0x07,
�Y�i�w�q8�Ӧr��
�o�̧ڭ̳]�w��@�Ӧ۩w�q�r����b0x00��m�]000�^,
�t�@�ө�b0x01��l�]001�^
------------------------------------------------*/
void Lcd_User_Chr(void)
{ //�Ĥ@�Ӧ۩w�q�r��
	LCD_Write_Com(0x40); //"01 000 000"  ��1��a�} (D7D6���a�}�]�w�R�O�Φ��ED5D4D3���r���s���m(0--7)�AD2D1D0���r����a�}(0--7)�^
	LCD_Write_Data(0x00); //"XXX 11111" ��1��ƾڡ]D7D6D5��XXX�A��ܬ����N��(�@���000�^�AD4D3D2D1D0���r����ƾ�(1-�I�G�A0-�����^
	LCD_Write_Com(0x41); //"01 000 001"  ��2��a�}
	LCD_Write_Data(0x04); //"XXX 10001" ��2��ƾ�
	LCD_Write_Com(0x42); //"01 000 010"  ��3��a�}
	LCD_Write_Data(0x0e); //"XXX 10101" ��3��ƾ�
	LCD_Write_Com(0x43); //"01 000 011"  ��4��a�}
	LCD_Write_Data(0x0e); //"XXX 10001" ��4��ƾ�
	LCD_Write_Com(0x44); //"01 000 100"  ��5��a�}
	LCD_Write_Data(0x0e); //"XXX 11111" ��5��ƾ�
	LCD_Write_Com(0x45); //"01 000 101"  ��6��a�}
	LCD_Write_Data(0x1f); //"XXX 01010" ��6��ƾ�
	LCD_Write_Com(0x46); //"01 000 110"  ��7��a�}
	LCD_Write_Data(0x04); //"XXX 11111" ��7��ƾ�
	LCD_Write_Com(0x47); //"01 000 111"  ��8��a�}
	LCD_Write_Data(0x00); //"XXX 00000" ��8��ƾ� 
	//�ĤG�Ӧ۩w�q�r��

	LCD_Write_Com(0x48); //"01 001 000"  ��1��a�}  
	LCD_Write_Data(0x03); //"XXX 00001" ��1��ƾ� 
	LCD_Write_Com(0x49); //"01 001 001"  ��2��a�}
	LCD_Write_Data(0x03); //"XXX 11011" ��2��ƾ�
	LCD_Write_Com(0x4a); //"01 001 010"  ��3��a�}
	LCD_Write_Data(0x00); //"XXX 11101" ��3��ƾ�
	LCD_Write_Com(0x4b); //"01 001 011"  ��4��a�}
	LCD_Write_Data(0x00); //"XXX 11001" ��4��ƾ�
	LCD_Write_Com(0x4c); //"01 001 100"  ��5��a�}
	LCD_Write_Data(0x00); //"XXX 11101" ��5��ƾ�
	LCD_Write_Com(0x4d); //"01 001 101"  ��6��a�}
	LCD_Write_Data(0x00); //"XXX 11011" ��6��ƾ�
	LCD_Write_Com(0x4e); //"01 001 110"  ��7��a�}
	LCD_Write_Data(0x00); //"XXX 00001" ��7��ƾ�
	LCD_Write_Com(0x4f); //"01 001 111"  ��8��a�}
	LCD_Write_Data(0x00); //"XXX 00000" ��8��ƾ� 
}

