/*-----------------------------------------------
  名稱：LCD1602
  論壇：www.doflye.net
  編寫：dennis
  日期：2020.10.8
  修改：無
  內容：通過矩陣鍵盤輸入，依次顯示0-F16中字元
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
#define KeyPort P1

unsigned char code dofly_code[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};//轉換成液晶顯示的字元
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
		s++;     
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
按鍵掃瞄函式，返回掃瞄鍵值
------------------------------------------------*/
unsigned char KeyScan(void)  //鍵盤掃瞄函式，使用行列反轉掃瞄法
{
	unsigned char cord_h,cord_l;//行列值中間變數
	KeyPort=0x0f;            //行線輸出全為0
	cord_h=KeyPort&0x0f;     //讀入列線值
	if(cord_h!=0x0f)    //先檢測有無按鍵按下
	{
		DelayMs(10);        //去抖
		if((KeyPort&0x0f)!=0x0f)
		{
			cord_h=KeyPort&0x0f;  //讀入列線值
			KeyPort=cord_h|0xf0;  //輸出目前列線值
			cord_l=KeyPort&0xf0;  //讀入行線值

			while((KeyPort&0xf0)!=0xf0);//等待鬆開並輸出

			return(cord_h+cord_l);//鍵盤最後組合碼值
		}
	}
	return(0xff);     //返回該值
}
/*------------------------------------------------
按鍵值處理函式，返回掃鍵值
------------------------------------------------*/
unsigned char KeyPro(void)
{
	switch(KeyScan())
	{
		case 0x7e:return 0;break;//0 按下相應的鍵顯示相對應的碼值
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
                    主函式
------------------------------------------------*/ 
void main(void) 
{ 
	unsigned char i,j,num; 

	LCD_Init(); 
	LCD_Write_Com(0x0F);//游標開，游標閃爍開

	LCD_Write_String(0,0,"Press the key !");

	while (1) 
	{  
		num=KeyPro();
		if(num!=0xff)
		{
			if((i==0)&&(j==0))//回到第一個字元時清屏
			LCD_Clear();//清屏
			LCD_Write_Char(0+i,0+j,dofly_code[num]);//依次顯示輸入字元
			i++;
			if(i==16)//如果第一行顯示滿，轉到第二行
			{
				i=0;j++;
				if(j==2) j=0;//如果2行都顯示滿，清屏後重新從第一行顯示
			}
		}
	}
}

