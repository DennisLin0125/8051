/*-----------------------------------------------
  名稱：LCD1602
  論壇：www.doflye.net
  編寫：dennis
  日期：2020.10.8
  修改：無
  內容：通過標準程式動態顯示字元 顯示游標和游標閃爍打開效果
  引腳定義如下：1-VSS 2-VDD 3-V0 4-RS 5-R/W 6-E 7-14 DB0-DB7 15-BLA 16-BLK
------------------------------------------------*/
#include<reg52.h> //包含標頭檔案，一般情況不需要改動，標頭檔案包含特殊功能暫存器的定義
#include<intrins.h>

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
 uS延時函式，含有輸入參數 unsigned char t，無返回值
 unsigned char 是定義無符號字元變數，其值的範圍是
 0~255 這裡使用晶振12M，精確延時請使用匯編,大致延時
 長度如下 T=tx2+5 uS 
------------------------------------------------*/
void DelayUs2x(unsigned char t)
{   
	while(--t);
}
/*------------------------------------------------
 mS延時函式，含有輸入參數 unsigned char t，無返回值
 unsigned char 是定義無符號字元變數，其值的範圍是
 0~255 這裡使用晶振12M，精確延時請使用匯編
------------------------------------------------*/
void DelayMs(unsigned char t)
{
     
	while(t--)
	{
		//大致延時1mS
		DelayUs2x(245);
		DelayUs2x(245);
	}
}
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
		LCD_Write_Com(0x80 + x);     
	}
	else 
	{     
		LCD_Write_Com(0xC0 + x);     
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
                    主函式
------------------------------------------------*/ 
void main(void) 
{ 
	unsigned char i; 
	unsigned char *pa,*pb; 

	LCD_Init(); 
	LCD_Write_Com(0x0f);//游標開，游標閃爍開
	while (1) 
	{  
		pa = "Welcome to";
		pb = "www.doflye.net";  
		LCD_Clear();  
		LCD_Write_String(2,0,pa);  //顯示2行資訊
		LCD_Write_String(1,1,pb);
		for(i=0;i<8;i++)            //延時2s左右
			DelayMs(250); 
		LCD_Clear();                //清屏顯示其他資訊
		i = 2; 
		while (*pa)                 //循環輸入方式顯示2行資訊
		{   
			LCD_Write_Char(i,0,*pa);   
			i++;   
			pa++;   
			DelayMs(250);   
		}  
		i=1;  
		while (*pb) 
		{   
			LCD_Write_Char(i,1,*pb);   
			i++;   
			pb++;   
			DelayMs(250);   
		}  

		DelayMs(250);  
	}
}

