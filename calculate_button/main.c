/*-----------------------------------------------
  名稱：液晶顯示計算器
  編寫：dennis
  日期：2020.9.22
  修改：無
  內容：整數之間運算，沒有小數所有除法得出的結果不正確，有負號運算，
------------------------------------------------*/
#include<reg52.h> //包含標頭檔案，一般情況不需要改動，標頭檔案包含特殊功能暫存器的定義
#include<stdio.h>
#include"display.h"
#include"delay.h"
#include"keyboard.h"


/*------------------------------------------------
                    主程式
------------------------------------------------*/
main()
{
	unsigned char num,i,sign;                   
	unsigned char temp[8];        //最大輸入8個
	bit firstflag;
	int a=0,b=0;
	unsigned char s;

	Init_Timer0();   //初始化定時器0
	while (1)         //主循環
	{


		num=KeyPro();  //掃瞄鍵盤
		if(num!=0xff)  //如果掃瞄是按鍵有效值則進行處理
		{ 
			if(i==0)    //輸入是第一個字元的時候需要把改行清空，方便觀看
			{
				for(s=0;s<8;s++) //賦值完成後把緩衝區清零，防止下次輸入影響結果
					TempData[s]=0;
			}
			if(('+'==num)|| (i==8) || ('-'==num) || ('x'==num)|| ('/'==num) || ('='==num))//輸入數字最大值8，輸入符號表示輸入結束
			{
				i=0;  //計數器復位

				if(firstflag==0)  //如果是輸入的第一個數據，賦值給a，並把標誌位置1，到下一個數據輸入時可以跳轉賦值給b
				{
					sscanf(temp,"%d",&a);//從一個字串輸入到變數
					firstflag=1;
				}
				else  
					sscanf(temp,"%d",&b);
				
				for(s=0;s<8;s++) //賦值完成後把緩衝區清零，防止下次輸入影響結果
					temp[s]=0;

				///////////////////////
				if(num!='=')      //判斷目前符號位並做相應處理
					sign=num;      //如果不是等號記下標誌位
				else
				{  
					firstflag=0;   //檢測到輸入=號，判斷上次讀入的符合
					switch(sign)
					{
						case '+':a=a+b;
						break;
						case '-':a=a-b;
						break;
						case 'x':a=a*b;
						break;
						case '/':a=a/b;
						break;
						default:break;
					}
					sprintf(temp,"%d",a);  //列印十進制到臨時緩衝區
					for(s=0;s<8;s++)       //由於列印的是ASCII碼值
					{ 
						if(temp[s]==0)      //所以需要轉換，如果為0表示null 數碼管上則不能顯示，所以賦值0
							TempData[s]=0;
						else if(temp[s]==0x2d)//表示負號，數碼管顯示負號 0x40
							TempData[s]=0x40;
						else 
							TempData[s]=dofly_DuanMa[temp[s]-'0'];//其他0-9負號則進行ASCII 到 數字處理，如目前是'3'，用'3'-'0'=3
																		//'3'的16進位制是0x33,'0'的16進位制是0x30			     
					}
					sign=0;a=b=0;            //用完后所有數據清零
					for(s=0;s<8;s++)
						temp[s]=0;
				}
			}

			else	if(i<16)
			{
				temp[i]=num+'0'; 
				TempData[i]=dofly_DuanMa[num];//輸出數據
				i++;   //輸入數值累加
			}

		}	
	}
}

