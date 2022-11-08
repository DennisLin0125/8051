/*-----------------------------------------------
  名稱：單個共陽數碼管模擬水流
  編寫：Dennis_Lin
  日期：2020.9.4
  修改：無
  內容：通過循環賦值給P1，讓數碼管顯示特定流動樣式
------------------------------------------------*/
#include<reg52.h> //包含標頭檔案，一般情況不需要改動，
//標頭檔案包含特殊功能暫存器的定義


void Delay(unsigned int t); //函式聲明
/*------------------------------------------------
                    主函式
------------------------------------------------*/
void main (void)
{
                  
	unsigned char i;  //定義一個無符號字元型區域性變數 i 取值範圍 0~255

	while (1)         //主循環
	{
		P1=0xfe;
		for(i=0;i<6;i++)   //加入 for循環，表明for循環大括號中的程式循環執行6次
		{
			Delay(10000);
			P1<<=1;
			P1|=0x01;         

		}

						//主循環中新增其他需要一直工作的程式
	}
}
/*------------------------------------------------
 延時函式，含有輸入參數 unsigned int t，無返回值
 unsigned int 是定義無符號整形變數，其值的範圍是
 0~65535
------------------------------------------------*/
void Delay(unsigned int t)
{
	while(--t);
}