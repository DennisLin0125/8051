
#ifndef __DS1302_H__
#define __DS1302_H__

#include <reg52.h>
#include<intrins.h>

sbit SCK=P1^4;		
sbit SDA=P1^5;		
sbit RST=P1^6;
//復位腳
#define RST_CLR	RST=0//電平置低
#define RST_SET	RST=1//電平置高


//雙向數據
#define IO_CLR	SDA=0//電平置低
#define IO_SET	SDA=1//電平置高
#define IO_R	SDA  //電平讀取


//時鐘訊號
#define SCK_CLR	SCK=0//時鐘訊號
#define SCK_SET	SCK=1//電平置高


#define ds1302_sec_add			0x80		//秒數據地址
#define ds1302_min_add			0x82		//分數據地址
#define ds1302_hr_add			0x84		//時數據地址
#define ds1302_date_add			0x86		//日數據地址
#define ds1302_month_add		0x88		//月數據地址
#define ds1302_day_add			0x8a		//星期數據地址
#define ds1302_year_add			0x8c		//年數據地址
#define ds1302_control_add		0x8e		//控制數據地址
#define ds1302_charger_add		0x90 					 
#define ds1302_clkburst_add		0xbe

extern unsigned char time_buf1[8];//空年月日時分秒周
extern unsigned char time_buf[8] ;//空年月日時分秒周
/*------------------------------------------------
           向DS1302寫入一位元組數據
------------------------------------------------*/
void Ds1302_Write_Byte(unsigned char addr, unsigned char d);
/*------------------------------------------------
           從DS1302讀出一位元組數據
------------------------------------------------*/
unsigned char Ds1302_Read_Byte(unsigned char addr) ;
/*------------------------------------------------
           向DS1302寫入時鐘數據
------------------------------------------------*/
void Ds1302_Write_Time(void) ;
/*------------------------------------------------
           從DS1302讀出時鐘數據
------------------------------------------------*/
void Ds1302_Read_Time(void)  ;
/*------------------------------------------------
                DS1302初始化
------------------------------------------------*/
void Ds1302_Init(void);

#endif
