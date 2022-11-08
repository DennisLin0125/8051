/*-----------------------------------------------
  ¦WºÙ¡GLCD1602
  ½s¼g¡Gdennis
  ¤é´Á¡G2020.10.9
  ­×§ï¡GµL
  ¤º®e¡G
  ¤Þ¸}©w¸q¦p¤U¡G1-VSS 2-VDD 3-V0 4-RS 5-R/W 6-E 7-14 DB0-DB7 15-BLA 16-BLK
------------------------------------------------*/
#include "1602.h"
#include "delay.h"

sbit RS = P2^4;   //©w¸q°ð 
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
              §P¦£¨ç¦¡
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
              ¼g¤J©R¥O¨ç¦¡
------------------------------------------------*/
void LCD_Write_Com(unsigned char com) 
{  
	// while(LCD_Check_Busy()); //¦£«hµ¥«Ý
	DelayMs(5);
	RS_CLR; 
	RW_CLR; 
	EN_SET; 
	DataPort= com; 
	_nop_(); 
	EN_CLR;
}
/*------------------------------------------------
              ¼g¤J¼Æ¾Ú¨ç¦¡
------------------------------------------------*/
void LCD_Write_Data(unsigned char Data) 
{ 
	//while(LCD_Check_Busy()); //¦£«hµ¥«Ý
	DelayMs(5);
	RS_SET; 
	RW_CLR; 
	EN_SET; 
	DataPort= Data; 
	_nop_();
	EN_CLR;
}

/*------------------------------------------------
                ²M«Ì¨ç¦¡
------------------------------------------------*/
void LCD_Clear(void) 
{ 
	LCD_Write_Com(0x01); 
	DelayMs(5);
}
/*------------------------------------------------
              ¼g¤J¦r¦ê¨ç¦¡
------------------------------------------------*/
void LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s) 
{     
	if (y == 0) 
	{     
		LCD_Write_Com(0x80 + x);     //ªí¥Ü²Ä¤@¦æ
	}
	else 
	{      
		LCD_Write_Com(0xC0 + x);      //ªí¥Ü²Ä¤G¦æ
	}        
	while (*s) 
	{     
		LCD_Write_Data( *s);     
		s ++;     
	}
}
/*------------------------------------------------
              ¼g¤J¦r¤¸¨ç¦¡
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
              ªì©l¤Æ¨ç¦¡
------------------------------------------------*/
void LCD_Init(void) 
{
	LCD_Write_Com(0x38);    /*Åã¥Ü¼Ò¦¡³]©w*/ 
	DelayMs(5); 
	LCD_Write_Com(0x38); 
	DelayMs(5); 
	LCD_Write_Com(0x38); 
	DelayMs(5); 
	LCD_Write_Com(0x38);  
	LCD_Write_Com(0x08);    /*Åã¥ÜÃö³¬*/ 
	LCD_Write_Com(0x01);    /*Åã¥Ü²M«Ì*/ 
	LCD_Write_Com(0x06);    /*Åã¥Ü´å¼Ð²¾°Ê³]©w*/ 
	DelayMs(5); 
	LCD_Write_Com(0x0C);    /*Åã¥Ü¶}¤Î´å¼Ð³]©w*/
}
/*------------------------------------------------   
³]©w¤G­Ó¦Û©w¸q¦r¤¸,LCD1602¤¤¦Û©w¸q¦r¤¸ªº¦a§}¬°0x00--0x07,
§Y¥i©w¸q8­Ó¦r¤¸
³o¸Ì§Ú­Ì³]©w§â¤@­Ó¦Û©w¸q¦r¤¸©ñ¦b0x00¦ì¸m¡]000¡^,
¥t¤@­Ó©ñ¦b0x01¦ì¤l¡]001¡^
------------------------------------------------*/
void Lcd_User_Chr(void)
{ //²Ä¤@­Ó¦Û©w¸q¦r¤¸
	LCD_Write_Com(0x40); //"01 000 000"  ²Ä1¦æ¦a§} (D7D6¬°¦a§}³]©w©R¥O§Î¦¡’ED5D4D3¬°¦r¤¸¦s©ñ¦ì¸m(0--7)¡AD2D1D0¬°¦r¤¸¦æ¦a§}(0--7)¡^
	LCD_Write_Data(0x00); //"XXX 11111" ²Ä1¦æ¼Æ¾Ú¡]D7D6D5¬°XXX¡Aªí¥Ü¬°¥ô·N¼Æ(¤@¯ë¥Î000¡^¡AD4D3D2D1D0¬°¦r¤¸¦æ¼Æ¾Ú(1-ÂI«G¡A0-º¶·À¡^
	LCD_Write_Com(0x41); //"01 000 001"  ²Ä2¦æ¦a§}
	LCD_Write_Data(0x04); //"XXX 10001" ²Ä2¦æ¼Æ¾Ú
	LCD_Write_Com(0x42); //"01 000 010"  ²Ä3¦æ¦a§}
	LCD_Write_Data(0x0e); //"XXX 10101" ²Ä3¦æ¼Æ¾Ú
	LCD_Write_Com(0x43); //"01 000 011"  ²Ä4¦æ¦a§}
	LCD_Write_Data(0x0e); //"XXX 10001" ²Ä4¦æ¼Æ¾Ú
	LCD_Write_Com(0x44); //"01 000 100"  ²Ä5¦æ¦a§}
	LCD_Write_Data(0x0e); //"XXX 11111" ²Ä5¦æ¼Æ¾Ú
	LCD_Write_Com(0x45); //"01 000 101"  ²Ä6¦æ¦a§}
	LCD_Write_Data(0x1f); //"XXX 01010" ²Ä6¦æ¼Æ¾Ú
	LCD_Write_Com(0x46); //"01 000 110"  ²Ä7¦æ¦a§}
	LCD_Write_Data(0x04); //"XXX 11111" ²Ä7¦æ¼Æ¾Ú
	LCD_Write_Com(0x47); //"01 000 111"  ²Ä8¦æ¦a§}
	LCD_Write_Data(0x00); //"XXX 00000" ²Ä8¦æ¼Æ¾Ú 
	//²Ä¤G­Ó¦Û©w¸q¦r¤¸

	LCD_Write_Com(0x48); //"01 001 000"  ²Ä1¦æ¦a§}  
	LCD_Write_Data(0x03); //"XXX 00001" ²Ä1¦æ¼Æ¾Ú 
	LCD_Write_Com(0x49); //"01 001 001"  ²Ä2¦æ¦a§}
	LCD_Write_Data(0x03); //"XXX 11011" ²Ä2¦æ¼Æ¾Ú
	LCD_Write_Com(0x4a); //"01 001 010"  ²Ä3¦æ¦a§}
	LCD_Write_Data(0x00); //"XXX 11101" ²Ä3¦æ¼Æ¾Ú
	LCD_Write_Com(0x4b); //"01 001 011"  ²Ä4¦æ¦a§}
	LCD_Write_Data(0x00); //"XXX 11001" ²Ä4¦æ¼Æ¾Ú
	LCD_Write_Com(0x4c); //"01 001 100"  ²Ä5¦æ¦a§}
	LCD_Write_Data(0x00); //"XXX 11101" ²Ä5¦æ¼Æ¾Ú
	LCD_Write_Com(0x4d); //"01 001 101"  ²Ä6¦æ¦a§}
	LCD_Write_Data(0x00); //"XXX 11011" ²Ä6¦æ¼Æ¾Ú
	LCD_Write_Com(0x4e); //"01 001 110"  ²Ä7¦æ¦a§}
	LCD_Write_Data(0x00); //"XXX 00001" ²Ä7¦æ¼Æ¾Ú
	LCD_Write_Com(0x4f); //"01 001 111"  ²Ä8¦æ¦a§}
	LCD_Write_Data(0x00); //"XXX 00000" ²Ä8¦æ¼Æ¾Ú 
}

