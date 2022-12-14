/*-----------------------------------------------
  名稱：定時器0 
  論壇：www.doflye.net
  編寫：dennis.lin
  日期：2020.9.7
  修改：無
  內容：通過定時產生1mS方波
------------------------------------------------*/
#include<reg52.h> //包含標頭檔案，一般情況不需要改動，標頭檔案包含特殊功能暫存器的定義

sbit OUT=P1^2;    //定義OUT輸出埠

/*------------------------------------------------
                    定時器初始化子程式
------------------------------------------------*/
void Init_Timer0(void)
{
	TMOD |= 0x01;	  //使用模式1，16位定時器，使用"|"符號可以在使用多個定時器時不受影響		     
	//TH0=0x00;	      //給定初值，這裡使用定時器最大值從0開始計數一直到65535溢出
	//TL0=0x00;
	EA=1;            //總中斷打開
	ET0=1;           //定時器中斷打開
	TR0=1;           //定時器開關打開
}
/*------------------------------------------------
                    主程式
------------------------------------------------*/
main()
{
	Init_Timer0();
	while(1);
}

/*------------------------------------------------
                 定時器中斷子程式
------------------------------------------------*/
void Timer0_isr(void) interrupt 1 using 1
{
	TH0=(65536-500)/256;		  //重新賦值 12M晶振計算，指令週期1uS，
	TL0=(65536-500)%256;         //1mS方波半個週期500uS，即定時500次
										

	OUT=~OUT;        //溢出然後輸出端取反  用示波器可看到方波輸出 

}