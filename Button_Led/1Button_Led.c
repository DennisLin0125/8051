/*-----------------------------------------------
  名稱：單個獨立按鍵檢測
  編寫：dennis.lin
  日期：2020.9.9
  修改：無
  內容：用於時刻檢測按鍵狀態並輸出指示
------------------------------------------------*/
#include<reg52.h> //包含標頭檔案，一般情況不需要改動，標頭檔案包含特殊功能暫存器的定義

sbit KEY=P3^3;  //定義按鍵輸入埠
sbit LED=P1^2;  //定義led輸出埠

/*------------------------------------------------
                    主函式
------------------------------------------------*/
void main (void)
{
                  
KEY=1; //按鍵輸入埠電平置高
	while (1)         //主循環
	{

		if(!KEY)  //如果檢測到低電平，說明按鍵按下
			LED=0;	
		else
			LED=1;  //這裡使用if判斷，如果按鍵按下led點亮，否則熄滅
						//上述4句可以用一句替代 LED=KEY;
							//主循環中新增其他需要一直工作的程式
	}
}