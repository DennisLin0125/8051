/*-----------------------------------------------
  名稱：雙色點陣
  編寫：dennis
  修改：無
  內容：使用hc595驅動雙色點陣
------------------------------------------------*/
#include<reg52.h>    //包含標頭檔案，一般情況不需要改動，標頭檔案包含特殊功能暫存器的定義
#include <intrins.h> 

//unsigned char  segout[8]={0,1,2,3,4,5,6,7}; //8列
unsigned char  segout[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80}; //8列
unsigned char code tab[]={//沙漏流動圖形表格
                        		
					//	0x81,0xC3,0xA5,0xD9,0xF9,0xE7,0xC3,0x81,
						
						0xFF,0x7E,0x3C,0x18,0x18,0x24,0x42,0xFF,
						0xFF,0x76,0x3C,0x18,0x18,0x24,0x4A,0xFF,
						0xFF,0x66,0x3C,0x18,0x18,0x24,0x5A,0xFF,
						0xFF,0x46,0x3C,0x18,0x18,0x24,0x7A,0xFF,
						0xFF,0x42,0x3C,0x18,0x18,0x24,0x7E,0xFF,
						0xFF,0x42,0x34,0x18,0x18,0x2C,0x7E,0xFF,
 						0xFF,0x42,0x24,0x18,0x18,0x3C,0x7E,0xFF,
						0x81,0xC3,0xA5,0x9B,0x9F,0xE7,0xC3,0x81


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
------------------------------------------------*/
void SendByte(unsigned char dat)
{    
	unsigned char i; 

	for(i=0;i<8;i++)
	{
		SRCLK=0;
		SER=dat&0x80;
		dat<<=1;
		SRCLK=1;
	}
         
}
/*------------------------------------------------
                發送雙位元組程式
     595級聯，n個595，就需要發送n位元組后鎖存
------------------------------------------------*/
void Send2Byte(unsigned char dat1,unsigned char dat2)
{    
   SendByte(dat1);
   SendByte(dat2);      
}
/*------------------------------------------------
                   595鎖存程式
		  595級聯發送數據后，鎖存有效
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
	unsigned char i,j,k;

	while(1)
	{
		for(j=0;j<8;j++)
			for(k=0;k<40;k++)//這個主要用於延時顯示動畫效果
				for(i=0;i<8;i++) //8列顯示
				{
					SendSeg(segout[i]);
					Send2Byte(~tab[i+j*8],0xff);
					Out595();
					DelayMs(1);
					Send2Byte(0xff,0xff); //防止重影
					Out595();
				}
		//另外一種顏色
		for(j=0;j<8;j++)
			for(k=0;k<40;k++)//這個主要用於延時顯示動畫效果
				for(i=0;i<8;i++) //8列顯示
				{
					SendSeg(segout[i]);
					Send2Byte(0xff,~tab[i+j*8]);
					Out595();
					DelayMs(1);
					Send2Byte(0xff,0xff); //防止重影
					Out595();
				}
	}
}
