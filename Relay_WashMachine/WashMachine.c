/*-----------------------------------------------
  名稱：獨立按鍵控制繼電器開關
  編寫：dennis
  日期：2020.10.5
  修改：無
  內容：對應的繼電器介面需用杜邦線連線到uln2003繼電器控制端
        通過控制2個繼電器循環工作狀態，模擬洗衣機間歇正反轉
        一般正轉3s，停止2s，然後反轉3s，停止2s，循環上述動作
------------------------------------------------*/
#include<reg52.h> //包含標頭檔案，一般情況不需要改動，標頭檔案包含特殊功能暫存器的定義

#define KeyPort P3

#define DataPort P0 //定義數據埠 程式中遇到DataPort 則用P0 替換
sbit LATCH1=P2^2;//定義鎖存使能埠 段鎖存
sbit LATCH2=P2^3;//                 位鎖存

sbit RELAY1 = P1^1;//定義繼電器訊號輸出埠1
sbit RELAY2 = P1^2;//定義繼電器訊號輸出埠2
/*------------------------------------------------
                  全域性變數
------------------------------------------------*/
unsigned char hour,minute,second;//定義時分秒

bit UpdateTimeFlag;//定義讀時間標誌

unsigned char code dofly_DuanMa[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};// 顯示段碼值0~9
unsigned char code dofly_WeiMa[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//分別對應相應的數碼管點亮,即位碼
unsigned char TempData[8]; //儲存顯示值的全域性變數
/*------------------------------------------------
                  函式聲明
------------------------------------------------*/
void DelayUs2x(unsigned char t);//us級延時函式聲明 
void DelayMs(unsigned char t); //ms級延時
void Display(unsigned char FirstBit,unsigned char Num);//數碼管顯示函式
unsigned char KeyScan(void);//鍵盤掃瞄
void Init_Timer0(void);//定時器初始化
void Stop(void);
void Clockwise (void);
void Anticlockwise(void);
/*------------------------------------------------
                    主函式
------------------------------------------------*/
void main (void)
{
	unsigned char num;                  
	Init_Timer0();    //初始化定時器0，主要用於數碼管動態掃瞄
	Stop();           //初始化洗衣機停止轉動
	while (1)         //主循環
	{
		num=KeyScan();    //循環呼叫按鍵掃瞄

		if(num==1)        //定時長度增加
		{
			if(minute<99)
				minute++;
		}
		else if(num==2)   //定時長度減小
		{
			if(minute>0)
				minute--;
		}
		if(UpdateTimeFlag==1)//定時重新整理顯示數據
		{
			UpdateTimeFlag=0;
			TempData[2]=dofly_DuanMa[minute/10];//分解顯示資訊，如要顯示68，則68/10=6  68%10=8 
			TempData[3]=dofly_DuanMa[minute%10];//分解顯示資訊，如要顯示68，則68/10=6  68%10=8  
			TempData[4]=0x40;
			TempData[5]=dofly_DuanMa[second/10];//分解顯示資訊，如要顯示68，則68/10=6  68%10=8 
			TempData[6]=dofly_DuanMa[second%10];//分解顯示資訊，如要顯示68，則68/10=6  68%10=8 

			if((minute==0)&&(second==0))//定時時間到，洗衣機停止轉動
			{
				Stop();
			} 
		}
	}
}
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
 顯示函式，用於動態掃瞄數碼管
 輸入參數 FirstBit 表示需要顯示的第一位，如賦值2表
 示從第三個數碼管開始顯示，如輸入0表示從第一個顯示。
 Num表示需要顯示的位數，如需要顯示99兩位數值則該值
 輸入2
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
	static unsigned int num,i,times;
	TH0=(65536-2000)/256;		  //重新賦值 2ms
	TL0=(65536-2000)%256;

	Display(0,8);                // 呼叫數碼管掃瞄
	i++;
	if(i==20)          //20ms更新一次
	{
		i=0;
		UpdateTimeFlag=1; //更新時間志位置1
	}
	num++;
	if(num==500)        //大致1s
	{
		num=0;

		///////////////////////////////////////
		//執行正反轉動作。
		switch(times)
		{
			case 1:Clockwise ();   break;//正轉3s
			case 4:Stop();         break;//停止2s
			case 6:Anticlockwise();break;//反轉3s
			case 9:Stop();         break;//停止2s
			case 11:times=0;       break;//清零循環上述動作
			default:break;
		}
		times++;
		///////////////////////////////////////////////////////
		if((minute!=0)||second)//如果分鐘和秒都為0，不進行計時
		second--;//秒減1
		if(second==0xff)//如果=0后再減1則賦值59，即00過後顯示59
		{
			second=59;
			if(minute>0)//倒計時條件
			{
				minute--;
			}

		}
	}
}

/*------------------------------------------------
按鍵掃瞄函式，返回掃瞄鍵值
------------------------------------------------*/
unsigned char KeyScan(void)
{
	unsigned char keyvalue;
	if(KeyPort!=0xff)
	{
		DelayMs(10);
		if(KeyPort!=0xff)
		{
			keyvalue=KeyPort;
			while(KeyPort!=0xff);
			switch(keyvalue)
			{
				case 0xfe:return 1;break;
				case 0xfd:return 2;break;
				case 0xfb:return 3;break;
				case 0xf7:return 4;break;
				case 0xef:return 5;break;
				case 0xdf:return 6;break;
				case 0xbf:return 7;break;
				case 0x7f:return 8;break;
				default:return 0;break;
			}
		}
	}
	return 0;
}

/*------------------------------------------------
                 順時針轉動
------------------------------------------------*/
void Clockwise (void)
{
	RELAY1=1;    //繼電器1吸合，接通正向線圈
	RELAY2=0;
}
/*------------------------------------------------
                 逆時針轉動
------------------------------------------------*/
void Anticlockwise(void)
{
	RELAY1=0;
	RELAY2=1;   //繼電器2吸合，接通反向線圈
}

/*------------------------------------------------
                 停止轉動
------------------------------------------------*/
void Stop(void)
{
	RELAY1=0;   
	RELAY2=0;
}
