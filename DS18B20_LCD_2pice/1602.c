/*-----------------------------------------------
  名稱：LCD1602
  編寫：dennis
  日期：2020.10.9
  修改：無
  內容：
  引腳定義如下：1-VSS 2-VDD 3-V0 4-RS 5-R/W 6-E 7-14 DB0-DB7 15-BLA 16-BLK
------------------------------------------------*/
#include "1602.h"
#include "delay.h"

sbit RS = P2^4;   //定義埠 
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
              判忙函式
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
              寫入命令函式
------------------------------------------------*/
void LCD_Write_Com(unsigned char com) 
{  
	// while(LCD_Check_Busy()); //忙則等待
	DelayMs(5);
	RS_CLR; 
	RW_CLR; 
	EN_SET; 
	DataPort= com; 
	_nop_(); 
	EN_CLR;
}
/*------------------------------------------------
              寫入數據函式
------------------------------------------------*/
void LCD_Write_Data(unsigned char Data) 
{ 
	//while(LCD_Check_Busy()); //忙則等待
	DelayMs(5);
	RS_SET; 
	RW_CLR; 
	EN_SET; 
	DataPort= Data; 
	_nop_();
	EN_CLR;
}

/*------------------------------------------------
                清屏函式
------------------------------------------------*/
void LCD_Clear(void) 
{ 
	LCD_Write_Com(0x01); 
	DelayMs(5);
}
/*------------------------------------------------
              寫入字串函式
------------------------------------------------*/
void LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s) 
{     
	if (y == 0) 
	{     
		LCD_Write_Com(0x80 + x);     //表示第一行
	}
	else 
	{      
		LCD_Write_Com(0xC0 + x);      //表示第二行
	}        
	while (*s) 
	{     
		LCD_Write_Data( *s);     
		s ++;     
	}
}
/*------------------------------------------------
              寫入字元函式
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
              初始化函式
------------------------------------------------*/
void LCD_Init(void) 
{
	LCD_Write_Com(0x38);    /*顯示模式設定*/ 
	DelayMs(5); 
	LCD_Write_Com(0x38); 
	DelayMs(5); 
	LCD_Write_Com(0x38); 
	DelayMs(5); 
	LCD_Write_Com(0x38);  
	LCD_Write_Com(0x08);    /*顯示關閉*/ 
	LCD_Write_Com(0x01);    /*顯示清屏*/ 
	LCD_Write_Com(0x06);    /*顯示游標移動設定*/ 
	DelayMs(5); 
	LCD_Write_Com(0x0C);    /*顯示開及游標設定*/
}
/*------------------------------------------------   
設定二個自定義字元,LCD1602中自定義字元的地址為0x00--0x07,
即可定義8個字元
這裡我們設定把一個自定義字元放在0x00位置（000）,
另一個放在0x01位子（001）
------------------------------------------------*/
void Lcd_User_Chr(void)
{ //第一個自定義字元
	LCD_Write_Com(0x40); //"01 000 000"  第1行地址 (D7D6為地址設定命令形式?ED5D4D3為字元存放位置(0--7)，D2D1D0為字元行地址(0--7)）
	LCD_Write_Data(0x00); //"XXX 11111" 第1行數據（D7D6D5為XXX，表示為任意數(一般用000），D4D3D2D1D0為字元行數據(1-點亮，0-熄滅）
	LCD_Write_Com(0x41); //"01 000 001"  第2行地址
	LCD_Write_Data(0x04); //"XXX 10001" 第2行數據
	LCD_Write_Com(0x42); //"01 000 010"  第3行地址
	LCD_Write_Data(0x0e); //"XXX 10101" 第3行數據
	LCD_Write_Com(0x43); //"01 000 011"  第4行地址
	LCD_Write_Data(0x0e); //"XXX 10001" 第4行數據
	LCD_Write_Com(0x44); //"01 000 100"  第5行地址
	LCD_Write_Data(0x0e); //"XXX 11111" 第5行數據
	LCD_Write_Com(0x45); //"01 000 101"  第6行地址
	LCD_Write_Data(0x1f); //"XXX 01010" 第6行數據
	LCD_Write_Com(0x46); //"01 000 110"  第7行地址
	LCD_Write_Data(0x04); //"XXX 11111" 第7行數據
	LCD_Write_Com(0x47); //"01 000 111"  第8行地址
	LCD_Write_Data(0x00); //"XXX 00000" 第8行數據 
	//第二個自定義字元

	LCD_Write_Com(0x48); //"01 001 000"  第1行地址  
	LCD_Write_Data(0x03); //"XXX 00001" 第1行數據 
	LCD_Write_Com(0x49); //"01 001 001"  第2行地址
	LCD_Write_Data(0x03); //"XXX 11011" 第2行數據
	LCD_Write_Com(0x4a); //"01 001 010"  第3行地址
	LCD_Write_Data(0x00); //"XXX 11101" 第3行數據
	LCD_Write_Com(0x4b); //"01 001 011"  第4行地址
	LCD_Write_Data(0x00); //"XXX 11001" 第4行數據
	LCD_Write_Com(0x4c); //"01 001 100"  第5行地址
	LCD_Write_Data(0x00); //"XXX 11101" 第5行數據
	LCD_Write_Com(0x4d); //"01 001 101"  第6行地址
	LCD_Write_Data(0x00); //"XXX 11011" 第6行數據
	LCD_Write_Com(0x4e); //"01 001 110"  第7行地址
	LCD_Write_Data(0x00); //"XXX 00001" 第7行數據
	LCD_Write_Com(0x4f); //"01 001 111"  第8行地址
	LCD_Write_Data(0x00); //"XXX 00000" 第8行數據 
}

