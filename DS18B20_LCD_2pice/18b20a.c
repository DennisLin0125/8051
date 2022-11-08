/*-----------------------------------------------
  名稱：18B20溫度感測器
  編寫：dennis
  日期：2020.10.9
  修改：無
  內容：18B20單線溫度檢測的應用樣例程式
------------------------------------------------*/
#include"delay.h"
#include"18b20a.h"
/*------------------------------------------------
                    18b20初始化
------------------------------------------------*/
bit Init_DS18B20_a(void)
{
	bit dat=0;
	DQ1 = 1;    //DQ1復位
	DelayUs2x(5);   //稍做延時
	DQ1 = 0;         //微控制器將DQ1拉低
	DelayUs2x(200); //精確延時 大於 480us 小於960us
	DelayUs2x(200);
	DQ1 = 1;        //拉高匯流排
	DelayUs2x(50); //15~60us 后 接收60-240us的存在脈衝
	dat=DQ1;        //如果x=0則初始化成功, x=1則初始化失敗
	DelayUs2x(25); //稍作延時返回
	return dat;
}

/*------------------------------------------------
                    讀取一個位元組
------------------------------------------------*/
unsigned char ReadOneChar_a(void)
{
	unsigned char i=0;
	unsigned char dat = 0;
	for (i=8;i>0;i--)
	{
		DQ1 = 0; // 給脈衝訊號
		dat>>=1;
		DQ1 = 1; // 給脈衝訊號
		if(DQ1)
			dat|=0x80;
		DelayUs2x(25);
	}
	return(dat);
}
/*------------------------------------------------
                    寫入一個位元組
------------------------------------------------*/
void WriteOneChar_a(unsigned char dat)
{
	unsigned char i=0;
	for (i=8; i>0; i--)
	{
		DQ1 = 0;
		DQ1 = dat&0x01;
		DelayUs2x(25);
		DQ1 = 1;
		dat>>=1;
	}
	DelayUs2x(25);
}

/*------------------------------------------------
                    讀取溫度
------------------------------------------------*/
unsigned int ReadTemperature_a(void)
{
	unsigned char a=0;
	unsigned int b=0;
	unsigned int t=0;
	Init_DS18B20_a();
	WriteOneChar_a(0xCC); // 跳過讀序號列號的操作
	WriteOneChar_a(0x44); // 啟動溫度轉換
	DelayMs(10);
	Init_DS18B20_a();
	WriteOneChar_a(0xCC); //跳過讀序號列號的操作 
	WriteOneChar_a(0xBE); //讀取溫度暫存器等（共可讀9個暫存器） 前兩個就是溫度
	a=ReadOneChar_a();   //低位
	b=ReadOneChar_a();   //高位

	b<<=8;
	t=a+b;

	return(t);
}
