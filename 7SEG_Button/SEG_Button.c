/*-----------------------------------------------
  名稱：單個共陽數碼管動態顯示變化數字
  編寫：dennis_lin	
  日期：2020.9.4
  修改：無
  內容：通過循環檢測P1口按鍵輸入值，然後用數碼管輸出，
        沒有按鍵按下時原值不變 
------------------------------------------------*/
#include<reg52.h> //包含標頭檔案，一般情況不需要改動，
//標頭檔案包含特殊功能暫存器的定義

unsigned char code dofly_table[10]={0xc0,0xf9,0xa4,0xb0,0x99,
                                    0x92,0x82,0xf8,0x80,0x90,}; 
									// 顯示數值表0-9

/*------------------------------------------------
                    主函式
------------------------------------------------*/
void main (void)
{              
	while (1)         //主循環
	{
		switch(P3)  //P3口作為獨立按鍵輸入端，檢測埠電平並做如下判斷                            
		{
			case 0xfe:
				P1=dofly_table[1];break;//0xfe = 1111 1110,
			//說明連線在P3.0埠的按鍵被按下,顯示對應的數字然後跳出循環
			case 0xfd:
				P1=dofly_table[2];break;//呼叫表中的第三個
			//元素 0xa4 下標 0 才表示陣列中的第一個元素
			case 0xfb:
				P1=dofly_table[3];break;
			case 0xf7:
				P1=dofly_table[4];break;
			case 0xef:
				P1=dofly_table[5];break;
			case 0xdf:
				P1=dofly_table[6];break;
			case 0xbf:
				P1=dofly_table[7];break;
			case 0x7f:
				P1=dofly_table[8];break;
			default:
				break;    //如果都沒按下，直接跳出
		}
		//主循環中新增其他需要一直工作的程式
	}
}