/*-----------------------------------------------
  名稱：定時器2
  論壇：www.doflye.net
  編寫：dennis.lin
  日期：2020.9.7
  修改：無
  內容：通過定時讓LED燈閃爍
------------------------------------------------*/
#include<reg52.h> //包含標頭檔案，一般情況不需要改動，標頭檔案包含特殊功能暫存器的定義

sbit LED=P1^0;    //定義LED埠


/*------------------------------------------------
                    定時器初始化子程式
------------------------------------------------*/
void TIM2Inital(void)
{
	RCAP2H = (65536-60000)/256;//晶振12M 60ms 16bit 自動過載
	RCAP2L = (65536-60000)%256;
	ET2=1;                     //打開定時器中斷
	EA=1;                      //打開總中斷
	TR2=1;                     //打開定時器開關
}
/*------------------------------------------------
                    主程式
------------------------------------------------*/
main()
{
	TIM2Inital();
	while(1);
}

/*------------------------------------------------
                 定時器中斷子程式
------------------------------------------------*/
void TIM2(void) interrupt 5 using 1//定時器2中斷
{

	TF2=0;
	LED=~LED;               //指示燈反相
   
}
