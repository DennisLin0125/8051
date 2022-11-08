/*-----------------------------------------------
  名稱：雙色點陣
  編寫：dennis
  修改：無
  內容：使用hc595驅動雙色點陣
------------------------------------------------*/
#include<reg52.h>    //包含標頭檔案，一般情況不需要改動，標頭檔案包含特殊功能暫存器的定義
#include <intrins.h> 


unsigned char  segout[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80}; //8列
unsigned char code tab[96]={
                        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //空格
                        0x00,0x7E,0xFF,0xC3,0xC3,0xFF,0x7E,0x00, //字元0
                        0x00,0x00,0x43,0xFF,0xFF,0x03,0x00,0x00, //字元1
                        0x00,0x63,0xC7,0xCF,0xDB,0xF3,0x63,0x00, //字元2
                        0x00,0x42,0xDB,0xDB,0xDB,0xFF,0x66,0x00, //字元3
                        0x00,0x3E,0x46,0xFF,0xFF,0x06,0x06,0x00, //字元4
                        0x00,0xF6,0xF7,0xD3,0xD3,0xDF,0xDE,0x00, //字元5
                        0x00,0x7E,0xFF,0xDB,0xDB,0xDF,0x4E,0x00, //字元6
                        0x00,0xC0,0xC0,0xC7,0xFF,0xF8,0xC0,0x00, //字元7
                        0x00,0xFF,0xFF,0xDB,0xDB,0xFF,0xFF,0x00, //字元8
                        0x00,0x72,0xFB,0xDB,0xDB,0xFF,0x7E,0x00, //字元9
                        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00   //空格
                        };
/*------------------------------------------------
                硬體埠定義
------------------------------------------------*/
sbit LATCH = P1^0; 
sbit SRCLK= P1^1;
sbit SER  = P1^2;


sbit LATCH_B = P2^2;
sbit SRCLK_B= P2^1;
sbit SER_B= P2^0;


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
                發送位元組程式
	   帶有方向參數，可以選擇從高位節寫入或者低位寫入
------------------------------------------------*/
void SendByte(unsigned char dat,bit direction)
{    
	unsigned char i,temp; 
	if(direction==0)
		temp=0x80;
	else
		temp=0x01;

	for(i=0;i<8;i++)
	{
		SRCLK=0;
		SER=dat&temp;
		
		if(direction==0)
			dat<<=1;
		else
			dat>>=1;
		
		SRCLK=1;
	}
         
}
/*------------------------------------------------
                發送雙位元組程式
------------------------------------------------*/
void Send2Byte(unsigned char dat1,unsigned char dat2,bit direction)
{    
   SendByte(dat1,direction);
   SendByte(dat2,direction);      
}
/*------------------------------------------------
                   595鎖存程式
------------------------------------------------*/
void Out595(void)
{
	LATCH=0;
	_nop_();
	LATCH=1;
}

/*------------------------------------------------
                發送位碼位元組程式
               使用另外一片單獨595
------------------------------------------------*/
void SendSeg(unsigned char dat)
{    
	unsigned char i; 

	for(i=0;i<8;i++)  //發送位元組
	{
		SRCLK_B=0;
		SER_B=dat&0x80;
		dat<<=1;
		SRCLK_B=1;
	}
	LATCH_B=0;    //鎖存
	_nop_();
	LATCH_B=1;
         
}
/*------------------------------------------------
                   主程式
------------------------------------------------*/
void main()
{
	unsigned char i,k,l,speed=10;
	while(1)
	{
		for(k=0;k<=88;k++)     //所有的字元個數
			for(l=speed;l>0;l--)    //延時長度,改變此值可以改變流動速度
				for(i=0;i<=7;i++) //8列顯示
				{
					SendSeg(segout[i]);
					Send2Byte(~(*(tab+i+k)),0xff,0);
					Out595();
					DelayMs(1);
					Send2Byte(0xff,0xff,0);//delay(10); //防止重影
					Out595();
				}


		for(k=0;k<=88;k++)     //所有的字元個數
			for(l=speed;l>0;l--)    //延時長度,改變此值可以改變流動速度
				for(i=0;i<=7;i++) //8列顯示
				{
					SendSeg(segout[7-i]);
					Send2Byte(0xff,~(*(tab+i+k)),1); //寫入位元組方向改變
					Out595();
					DelayMs(1);
					Send2Byte(0xff,0xff,0);//delay(10); //防止重影
					Out595();
				}
	}
}