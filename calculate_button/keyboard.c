/*-----------------------------------------------
  名稱：矩陣鍵盤
  編寫：dennis
  日期：2020.10.10
  修改：無
  內容：
------------------------------------------------*/
#include<reg52.h> //包含標頭檔案，一般情況不需要改動，標頭檔案包含特殊功能暫存器的定義
#include"keyboard.h"
#include"delay.h"

#define KeyPort P1

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
           可以根據需要改變返回值

  			| 1 | 2 | 3 | + |  
  			| 4 | 5 | 6 | - |  
  			| 7 | 8 | 9 | * |  
  			| 0 | . | = | / | 
------------------------------------------------*/
unsigned char KeyPro(void)
{
	switch(KeyScan())
	{
		case 0x7e:return 1  ;break;//0 按下相應的鍵顯示相對應的碼值
		case 0x7d:return 2  ;break;//1
		case 0x7b:return 3  ;break;//2
		case 0x77:return '+';break;//3

		case 0xbe:return 4  ;break;//4
		case 0xbd:return 5  ;break;//5
		case 0xbb:return 6  ;break;//6
		case 0xb7:return '-';break;//7

		case 0xde:return 7  ;break;//8
		case 0xdd:return 8  ;break;//9
		case 0xdb:return 9  ;break;//a
		case 0xd7:return 'x';break;//b

		case 0xee:return 0  ;break;//c
		case 0xed:return '.';break;//d
		case 0xeb:return '=';break;//e
		case 0xe7:return '/';break;//f
		default:return 0xff;break;
	}
}