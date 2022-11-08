
#include "ds1302.h"

unsigned char time_buf1[8] = {20,10,6,5,12,55,00,6};//空年月日時分秒周
unsigned char time_buf[8] ;                         //空年月日時分秒周
/*------------------------------------------------
           向DS1302寫入一位元組數據
------------------------------------------------*/
void Ds1302_Write_Byte(unsigned char addr, unsigned char d)
{

	unsigned char i;
	RST_SET;	

	//寫入目標地址：addr
	addr = addr & 0xFE;     //最低位置零
	for (i = 0; i < 8; i ++) 
	{ 
		if (addr & 0x01) 
		{
			IO_SET;
		}
		else 
		{
			IO_CLR;
		}
		SCK_SET;
		SCK_CLR;
		addr = addr >> 1;
	}

	//寫入數據：d
	for (i = 0; i < 8; i ++) 
	{
		if (d & 0x01) 
		{
			IO_SET;
		}
		else 
		{
			IO_CLR;
		}
		SCK_SET;
		SCK_CLR;
		d = d >> 1;
	}
	RST_CLR;					//停止DS1302匯流排
}
/*------------------------------------------------
           從DS1302讀出一位元組數據
------------------------------------------------*/

unsigned char Ds1302_Read_Byte(unsigned char addr) 
{

	unsigned char i;
	unsigned char temp;
	RST_SET;	

	//寫入目標地址：addr
	addr = addr | 0x01;//最低位置高
	for (i = 0; i < 8; i ++) 
	{

		if (addr & 0x01) 
		{
			IO_SET;
		}
		else 
		{
			IO_CLR;
		}
		SCK_SET;
		SCK_CLR;
		addr = addr >> 1;
	}

	//輸出數據：temp
	for (i = 0; i < 8; i ++) 
	{
		temp = temp >> 1;
		if (IO_R) 
		{
			temp |= 0x80;
		}
		else 
		{
			temp &= 0x7F;
		}
		SCK_SET;
		SCK_CLR;
	}

	RST_CLR;	//停止DS1302匯流排
	return temp;
}

/*------------------------------------------------
           向DS1302寫入時鐘數據
------------------------------------------------*/
void Ds1302_Write_Time(void) 
{
     
	unsigned char i,tmp;
	for(i=0;i<8;i++)
	{                  //BCD處理
		tmp=time_buf1[i]/10;
		time_buf[i]=time_buf1[i]%10;
		time_buf[i]=time_buf[i]+tmp*16;
	}
	Ds1302_Write_Byte(ds1302_control_add,0x00);			//關閉防寫 
	Ds1302_Write_Byte(ds1302_sec_add,0x80);				//暫停 
	//Ds1302_Write_Byte(ds1302_charger_add,0xa9);			//涓流充電 
	Ds1302_Write_Byte(ds1302_year_add,time_buf[1]);		//年 
	Ds1302_Write_Byte(ds1302_month_add,time_buf[2]);	//月 
	Ds1302_Write_Byte(ds1302_date_add,time_buf[3]);		//日 
	Ds1302_Write_Byte(ds1302_day_add,time_buf[7]);		//周 
	Ds1302_Write_Byte(ds1302_hr_add,time_buf[4]);		//時 
	Ds1302_Write_Byte(ds1302_min_add,time_buf[5]);		//分
	Ds1302_Write_Byte(ds1302_sec_add,time_buf[6]);		//秒
	Ds1302_Write_Byte(ds1302_day_add,time_buf[7]);		//周 
	Ds1302_Write_Byte(ds1302_control_add,0x80);			//打開防寫 
}

/*------------------------------------------------
           從DS1302讀出時鐘數據
------------------------------------------------*/
void Ds1302_Read_Time(void)  
{ 
	unsigned char i,tmp;
	time_buf[1]=Ds1302_Read_Byte(ds1302_year_add);		//年 
	time_buf[2]=Ds1302_Read_Byte(ds1302_month_add);		//月 
	time_buf[3]=Ds1302_Read_Byte(ds1302_date_add);		//日 
	time_buf[4]=Ds1302_Read_Byte(ds1302_hr_add);		//時 
	time_buf[5]=Ds1302_Read_Byte(ds1302_min_add);		//分 
	time_buf[6]=(Ds1302_Read_Byte(ds1302_sec_add))&0x7F;//秒 
	time_buf[7]=Ds1302_Read_Byte(ds1302_day_add);		//周 


	for(i=0;i<8;i++)
	{           //BCD處理
		tmp=time_buf[i]/16;
		time_buf1[i]=time_buf[i]%16;
		time_buf1[i]=time_buf1[i]+tmp*10;
	}
}

/*------------------------------------------------
                DS1302初始化
------------------------------------------------*/
void Ds1302_Init(void)
{
	
	RST_CLR;			//RST腳置低
	SCK_CLR;			//SCK腳置低
	Ds1302_Write_Byte(ds1302_sec_add,0x00);				 
}
