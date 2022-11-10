//-----------------函數宣告,變數定義-----------------------------

#include <reg52.h>

#define COUNT 10                 //定義接收緩衝區大小
#define Slaver_Num 10

#define uchar_8 unsigned char

sbit RE_DE=P1^0;

uchar_8 bdata flag;              //在可位元定址去定義一個旗標變數
sbit time_over_flag = flag^0;    //接收超時旗標 

uchar_8 buffer[COUNT];           //定義緩衝區
uchar_8 point;                   //定義緩衝區位置指示

uchar_8 Slave_AD[Slaver_Num];    //定義有效位置存放區
uchar_8 ADD_num;                 //有效位址個數
uchar_8 idata count_10ms;        //用於表示有多少次10ms中斷
uchar_8 idata send_data[7]=
{
	0x31,0x32,0x33,0x34,0x35,0x36,0x37  //與定義發送資料,共7位元
};
//--------------------------------------------------------------
// 函數名稱 : UART_init()串列初始化函數
// 函數功能 : 在系統時脈為11.0592MHz時,設定串列資料傳輸率為9600
//            串列接收中斷允許,發送中斷禁止,設定計時器中斷允許
//--------------------------------------------------------------
void UART_init()
{
	//初始化設置
	SCON=0x58;    //0101 1000 選擇串列工作模式為1,接收允許,TB8=1
	TMOD=0x21;    //0010 0001 計時器1模式2,計時器0模式1
	TR1=1;        //啟動計時器T1
	ES=1;         //允許串列中斷
	PS=1;         //設定串列中斷優先等級
	
	//初始化計時器1
	TH1=0xfd;     //1111 1101設定鮑率9600(11.0592MHz)
	ET1=0;        //Timer1中斷禁止
}
//------------------------------------------------------------------
// 函數名稱 : timer0_init() 初始化Timer0
// 函數功能 : 設置Timer0工作模式
//------------------------------------------------------------------
void timer0_init()
{
	time_over_flag=0;
	count_10ms=0;
	ADD_num=0;
	TL0=0x0F0;     //T0用於產生10ms的中斷
	TH0=0x0D8;     //50次T0中斷產生1次超時溢位
	ET0=1;         //允許Timer0中斷
}
//--------------------------------------------------------------------
// 函數名稱 : system_init() 系統初始化
// 函數功能 : 呼叫串列埠,計時器初始化函數,完成系統初始化
//---------------------------------------------------------------------
void system_init(void)
{
	//系統總設置
	UART_init();
	timer0_init();
	EA=1;          //允許單晶片中斷
}
//-----------------------------------------------------------------------
// 函數名稱 : com_interrupt() 系統初始化
// 函數功能 : 接收包括起始位"S"在內的10bit資料到緩衝區
//-----------------------------------------------------------------------
com_interrupt(void) interrupt 4 using 3
{
	uchar_8 RECEIVR_buffer;
	if(RI)         						//處理接收中斷
	{
		RI=0;       					//清出接收中斷旗標位元
		RECEIVR_buffer=SBUF; 				//接收串列埠資料
		if(point==0)         				//如果還沒有接收到起始位元
		{
			if(RECEIVR_buffer==0xFE) 		//判斷是否起始旗標位元
			{
				buffer[point++]=RECEIVR_buffer; //把接收到的資料放入緩衝區
			}
			else
				point=0;                        //不是,繼續等待起始位元
		}
		else if(point>0&&point<10)                      // 0<point<10  判斷是否夠接收10bit資料

			buffer[point++]=RECEIVR_buffer;         // 不夠,把接收到的資料放入緩衝區

		else if(point==10)
		{
			if(RECEIVR_buffer==0xEF)                //判斷結束旗標是否正確
			{
				buffer[point]=RECEIVR_buffer;   //把接收到的資料放入緩衝區
				Slave_AD[ADD_num++]=buffer[2];  //把接收到的位址放入位址記憶體
								//表示該位址有有效設備
			}
			else
				point=0;                        //不是,繼續等待起始位元
		}
		else
			point=0;                            	//緩衝區已滿,清除緩衝區內資料重新接收
	}
	if(TI)                                      		//處理發送中斷
	{
		TI=0;
	}
}
//---------------------------------------------------------------------------------------
// 函數名稱 : timer0_interrupt() 
// 函數功能 : 計時器T0中斷服務程式
// 函數說明 : T0每10ms中斷一次,連續中斷50次後置time_over_flag=1;
//---------------------------------------------------------------------------------------
timer0_interrupt(void) interrupt 1 using 2
{
	count_10ms++;
	if(count_10ms==50)
	{
		ET0=0;                 //關閉計時器T0中斷
		TR0=0;                 //停止計時器T0
		time_over_flag=1;      //設置接收超時旗標
		count_10ms=0x00;       //10ms計數器重置
	}
	else
	{
		TL0=0x0F0;             //計數器重置初值
		TH0=0x0D8;
	}
}
//---------------------------------------------------------------------------------------
// 函數名稱 : COM_send() 
// 函數功能 : 把資料緩衝區的10bit資料發送出去
//---------------------------------------------------------------------------------------
void COM_send(void)
{
	RE_DE=1;                        //設置MAX483進入發送狀態
	for(point=0;point<=10;point++)  //連續發送10bit資料,把緩衝區的資料送到串列埠
	{
		SBUF=buffer[point];
		TI=0;
	}
	RE_DE=0;                        //設置MAX483進入接收狀態
}
//--------------------------------------------------------------------------------------
// 函數名稱 : write_buffer() 
// 函數功能 : 寫發送緩衝區10bit資料
//--------------------------------------------------------------------------------------
void write_buffer(uchar_8 slaver_add)
{
	uchar_8 i;
	TB8=1;                            //打開多機通信方式
	buffer[0]=0xFE;
	buffer[1]=slaver_add;
	for(i=2;i<9;i++)                  //連續發送10bit資料,把緩衝區的資料送到串列埠
	{
		buffer[i]=send_data[i-2];
		
	}
	buffer[9]=0xEF;
}
//------------------------------------------------------------------------------------
// 函數名稱 : 主函數 
// 函數功能 : 呼叫各子函數,完成通信過程
//------------------------------------------------------------------------------------
void main(void)
{
	uchar_8 i=0;                   
	system_init();                    //系統初始化
	do                                //查詢0~10號位址有沒有對應設備
	{
		write_buffer(i++);        //寫查詢第i號設備的發送資訊
		COM_send();               //呼叫發送函數,完成發送
		timer0_init();            //完成一次查詢,重新初始timer0,準備下一次查詢
	}while(time_over_flag&&i<10);
}