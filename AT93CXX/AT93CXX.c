//--------------函數宣告-------------
#include <reg52.h>
#include <intrins.h>

#define READ_C 0x02
#define WRITE_C 0x01
#define ERASE_C 0x03
#define EWEN_C 0x00
#define EWDS_C 0x00
#define WRAL_C 0x00
#define ERAL_C 0x00
#define uint_16 unsigned int
#define uchar_8 unsigned char

sbit CS=P1^0;
sbit CLK=P1^1;
sbit DI=P1^2;
sbit DO=P1^3;
//---------------------------------
// 函數名稱 : send1
// 函數功能 : 發送1位元資料1
//---------------------------------
void send_1(void)
{
    DI=1;
    CLK=1;
    _nop_();
    CLK=0;
    _nop_();
}
//---------------------------------
// 函數名稱 : send0
// 函數功能 : 發送1位元資料0
//---------------------------------
void send_0(void)
{
    DI=0;
    CLK=1;
    _nop_();
    CLK=0;
    _nop_();
}
//---------------------------------
// 函數名稱 : start_operation
// 函數功能 : 開始操作
//---------------------------------
void start_operation(uchar_8 op_code)
{
    CS=1;

    send_1();            //起始位元

    if(op_code & 0x02)   //操作碼
        send_1();
    else
        send_0();

    if(op_code & 0x01)
        send_1();
    else
        send_0();
}
//---------------------------------
// 函數名稱 : send_byte
// 輸入參數 : dd,待發送的資料
// 函數功能 : 送1byte資料
//---------------------------------
void send_byte(uchar_8 dd)
{
    uchar_8 i;
    for(i=0;i<8;i++)
    {
        if(dd>=0x80)
            send_1();
        else
            send_0();
        dd<<=1;
    }
}
//---------------------------------
// 函數名稱 : receive_byte
// 輸入參數 : dd,接收的資料
// 函數功能 : 接收1byte資料
//---------------------------------
uchar_8 receive_byte(void)
{
    uchar_8 i,dd;
    DO=1;
    for(i=0;i<8;i++)
    {
        dd<<=1;
        CLK=1;
        CLK=0;
        if(DO)
            dd|=1;
    }
    return(dd);
}
//---------------------------------
// 函數名稱 : EWEN
// 函數功能 : 擦/寫允許命令
// 通電重置後AT93CXXX首先將處於擦/寫不允許狀態
// 故該晶片指令必須在程式編寫前執行 
//---------------------------------
void EWEN(void)
{
    _nop_();
    CS=1;
    start_operation(EWEN_C);
    send_byte(0xc0);
    CS=0;
}
//---------------------------------
// 函數名稱 : EWDS
// 函數功能 : 擦寫截止
// 使用該指令可對寫入的資料進行保護 
//---------------------------------
void EWDS(void)
{
    _nop_();
    CS=1;
    start_operation(EWDS_C);
    send_byte(0x00);
    CS=0;
}
//---------------------------------
// 函數名稱 : ERAL
// 函數功能 : 晶片擦除
// 該指令可將整個記憶體陣列置為1
//---------------------------------
void ERAL(void)
{
    _nop_();
    CS=1;
    start_operation(ERAL_C);
    send_byte(0x80);
    CS=0;
}
//---------------------------------
// 函數名稱 : READ_BYTE
// 函數功能 : 從儲存區address讀1byte資料
//---------------------------------
uchar_8 READ_BYTE(uchar_8 address)
{
    uchar_8 dat;
    CS=CLK=0;
    CS=1;
    start_operation(READ_C);
    send_byte(address);
    dat=receive_byte();
    CS=0;
    return(dat);
}
//---------------------------------
// 函數名稱 : WRITE_BYTE
// 輸入參數 : address,dat
// 返回值 : 1或0,表示是否成功寫入
// 函數功能 : 寫1byte資料到儲存區
//---------------------------------
bit WRITE_BYTE(uchar_8 address,uchar_8 dat)
{
    uchar_8 e,time,temp=address;
    e=0;
    while (e<3)
    {
        CS=CLK=0;
        CS=1;
        EWEN();
        _nop_();
        CS=1;
        _nop_();
        start_operation(WRITE_C);
        send_byte(address);
        send_byte(dat);
        CS=0;
        _nop_();
        CS=1;
        time=0;
        DO=1;
        while (1)
        {
            if(DO==1)
            {
                break;
            }
                
            if (time>20)
            {
                break;
            }
            time++;
        }
        CS=0;
        if (READ_BYTE(temp)==dat)
        {
            return(0);
        }
        e++;   
    }
    return(1);
}