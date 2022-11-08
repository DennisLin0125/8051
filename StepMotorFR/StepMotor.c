/*-----------------------------------------------
  名稱：步進電機
  論壇：www.doflye.net
  編寫：Dennis
  日期：2020.10.5
  修改：無
  內容：本程式用於測試4相步進電機常規驅動 含正反轉 使用1相勵磁
        

 ------------------------------------------------*/

#include <reg52.h>



sbit A1=P1^0; //定義步進電機連線埠
sbit B1=P1^1;
sbit C1=P1^2;
sbit D1=P1^3;


#define Coil_A1 {A1=1;B1=0;C1=0;D1=0;}//A相通電，其他相斷電
#define Coil_B1 {A1=0;B1=1;C1=0;D1=0;}//B相通電，其他相斷電
#define Coil_C1 {A1=0;B1=0;C1=1;D1=0;}//C相通電，其他相斷電
#define Coil_D1 {A1=0;B1=0;C1=0;D1=1;}//D相通電，其他相斷電
#define Coil_AB1 {A1=1;B1=1;C1=0;D1=0;}//AB相通電，其他相斷電
#define Coil_BC1 {A1=0;B1=1;C1=1;D1=0;}//BC相通電，其他相斷電
#define Coil_CD1 {A1=0;B1=0;C1=1;D1=1;}//CD相通電，其他相斷電
#define Coil_DA1 {A1=1;B1=0;C1=0;D1=1;}//D相通電，其他相斷電
#define Coil_OFF {A1=0;B1=0;C1=0;D1=0;}//全部斷電

unsigned char Speed;
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
                    主函式
------------------------------------------------*/
main()
{
 unsigned int i=512;//旋轉一週時間
 Speed=8;
 Coil_OFF
 while(i--)  //正向
  {         
     Coil_A1                //遇到Coil_A1  用{A1=1;B1=0;C1=0;D1=0;}代替
     DelayMs(Speed);         //改變這個參數可以調整電機轉速 ,
                             //數字越小，轉速越大,力矩越小
     Coil_B1
     DelayMs(Speed);
     Coil_C1
     DelayMs(Speed);
     Coil_D1
     DelayMs(Speed);
  }
  Coil_OFF
  i=512;
  while(i--)//反向
  {
     Coil_D1                //遇到Coil_A1  用{A1=1;B1=0;C1=0;D1=0;}代替
     DelayMs(Speed);         //改變這個參數可以調整電機轉速 ,
	                         //數字越小，轉速越大,力矩越小
     Coil_C1
     DelayMs(Speed);
     Coil_B1
     DelayMs(Speed);
     Coil_A1
     DelayMs(Speed);
  }
}
