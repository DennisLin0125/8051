//-----------------函數宣告,變數定義-----------------------------

#include <reg52.h>

#define COUNT 10                 //定義接收緩衝區大小
#define ADD 5

#define uchar_8 unsigned char

sbit RE_DE=P1^0;

uchar_8 buffer[COUNT];           //定義接收緩衝區
uchar_8 point;                   //定義接收資料個數指示變數

//------------------------------------------------------------
// 函數名稱 : UART_init()串列初始化函數
// 函數功能 : 在系統時脈為11.0592MHz時,設定串列資料傳輸率為9600
//            串列接收中斷允許,發送中斷禁止,設定計時器中斷允許
//------------------------------------------------------------
void UART_init()
{
	              //初始化串列埠和資料傳輸速率產生器
	SCON=0xF0;    //1111 0000 選擇串列工作模式為3,接收允許
	TMOD=0x21;    //0010 0001 計時器1模式2,計時器0模式1
	TR1=1;        //啟動計時器T1
	SM2=1;        //設備處於位址監聽狀態
	ET1=0;        //Timer1中斷禁止
	ES=1;         //允許串列中斷
	PS=1;         //設定串列中斷優先等級
	EA=1;         //單晶片允許中斷
}
//--------------------------------------------------------------
// 函數名稱 : com_interrupt() 系統初始化
// 函數功能 : 接收包括起始位0xFE,位址位和終止位0xEF在內的10bit資料
//            到緩衝區,位址不匹配,則接收到無資料,不寫到緩衝區
//--------------------------------------------------------------
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
				buffer[point++]=RECEIVR_buffer; //起始正確,接收起始位元
			}
			else
			{
				point=0;                        //不是,繼續等待起始位元
			}
		}
		else if(point==1)                    		//是否地址位
		{
			if(RECEIVR_buffer==ADD )           	//判斷地址是否匹配
			{
				buffer[point++]=RECEIVR_buffer; //位址匹配,接收開始接收
			}
			else
			{
				point=0;                        //不是,繼續等待起始位元
			}
		}
		else if(point>0&&point<10)            		// 0<point<10  判斷是否夠接收10bit資料
		{
			buffer[point++]=RECEIVR_buffer;    	// 不夠,把接收到的資料放入緩衝區
		}
		else
		{
			point=0;                           	//緩衝區已滿,清除緩衝區內資料重新接收
		}
	}
	if(TI)                                   		//處理發送中斷
	{
		TI=0;
	}
}
//----------------------------------------------
// 函數名稱 : COM_send() 
// 函數功能 : 把資料緩衝區的10bit資料發送出去
//----------------------------------------------
void COM_send(void)
{
	RE_DE=1;                             //設置MAX483進入發送狀態
	for(point=0;point<=10,TI=1;point++)  //連續發送10bit資料,把緩衝區的資料送到串列埠
	{
		SBUF=buffer[point];
		TI=0;
	}
	RE_DE=0;                             //設置MAX483進入接收狀態
}
//----------------------------------------------
// 函數名稱 : 主函數 
// 函數功能 : 呼叫各子函數,完成通信過程
//----------------------------------------------
void main(void)
{                   
	UART_init();                   //串列初始化
	do                            
	{	
	}while(point!=10);             //迴圈判斷是否接收完成
	COM_send();                    //位址匹配,資料完整接收,呼叫發送函數,通知主機
	SM2=0;                         //設置為點對點傳送模式
}
