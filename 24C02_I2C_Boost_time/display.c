#include"display.h"
#include"delay.h"

#define DataPort P0 //定義數據埠 程式中遇到DataPort 則用P0 替換
//sbit LATCH1=P2^0;//定義鎖存使能埠 段鎖存
//sbit LATCH2=P2^3;//                 位鎖存

unsigned char code dofly_DuanMa[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};// 顯示段碼值0~9
unsigned char code dofly_WeiMa[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//分別對應相應的數碼管點亮,即位碼
unsigned char TempData[8]; //儲存顯示值的全域性變數


/*------------------------------------------------
 顯示函式，用於動態掃瞄數碼管
 輸入參數 FirstBit 表示需要顯示的第一位，如賦值2表示從第三個數碼管開始顯示
 如輸入0表示從第一個顯示。
 Num表示需要顯示的位數，如需要顯示99兩位數值則該值輸入2
------------------------------------------------*/
void Display(unsigned char FirstBit,unsigned char Num)
{
      static unsigned char i=0;
	  

	   DataPort=0;   //清空數據，防止有交替重影
       LATCH1=1;     //段鎖存
       LATCH1=0;

       DataPort=dofly_WeiMa[i+FirstBit]; //取位碼 
       LATCH2=1;     //位鎖存
       LATCH2=0;

       DataPort=TempData[i]; //取顯示數據，段碼
       LATCH1=1;     //段鎖存
       LATCH1=0;
       
	   i++;
       if(i==Num)
	      i=0;


}
/*------------------------------------------------
                    定時器初始化子程式
------------------------------------------------*/
void Init_Timer0(void)
{
 TMOD |= 0x01;	  //使用模式1，16位定時器，使用"|"符號可以在使用多個定時器時不受影響		     
 //TH0=0x00;	      //給定初值
 //TL0=0x00;
 EA=1;            //總中斷打開
 ET0=1;           //定時器中斷打開
 TR0=1;           //定時器開關打開
}
/*------------------------------------------------
                 定時器中斷子程式
------------------------------------------------*/
void Timer0_isr(void) interrupt 1 
{
 TH0=(65536-2000)/256;		  //重新賦值 2ms
 TL0=(65536-2000)%256;
 
 Display(0,8);

}

