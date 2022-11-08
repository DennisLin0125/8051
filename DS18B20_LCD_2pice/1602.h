/*-----------------------------------------------
  名稱：LCD1602.h
  編寫：shifang
  日期：2009.5
  修改：無
  內容：
  引腳定義如下：1-VSS 2-VDD 3-V0 4-RS 5-R/W 6-E 7-14 DB0-DB7 15-BLA 16-BLK
------------------------------------------------*/
#include<reg52.h> //包含標頭檔案，一般情況不需要改動，標頭檔案包含特殊功能暫存器的定義
#include<intrins.h>

#ifndef __1602_H__
#define __1602_H__


 bit LCD_Check_Busy(void) ;

 void LCD_Write_Com(unsigned char com) ;

 void LCD_Write_Data(unsigned char Data) ;

 void LCD_Clear(void) ;

 void LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s) ;

 void LCD_Write_Char(unsigned char x,unsigned char y,unsigned char Data) ;

 void LCD_Init(void) ;

 void Lcd_User_Chr(void);
   
#endif
