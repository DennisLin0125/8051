
#ifndef __DS1302_H__
#define __DS1302_H__

#include <reg52.h>
#include<intrins.h>

sbit SCK=P1^4;		
sbit SDA=P1^5;		
sbit RST=P1^6;
//�_��}
#define RST_CLR	RST=0//�q���m�C
#define RST_SET	RST=1//�q���m��


//���V�ƾ�
#define IO_CLR	SDA=0//�q���m�C
#define IO_SET	SDA=1//�q���m��
#define IO_R	SDA  //�q��Ū��


//�����T��
#define SCK_CLR	SCK=0//�����T��
#define SCK_SET	SCK=1//�q���m��


#define ds1302_sec_add			0x80		//��ƾڦa�}
#define ds1302_min_add			0x82		//���ƾڦa�}
#define ds1302_hr_add			0x84		//�ɼƾڦa�}
#define ds1302_date_add			0x86		//��ƾڦa�}
#define ds1302_month_add		0x88		//��ƾڦa�}
#define ds1302_day_add			0x8a		//�P���ƾڦa�}
#define ds1302_year_add			0x8c		//�~�ƾڦa�}
#define ds1302_control_add		0x8e		//����ƾڦa�}
#define ds1302_charger_add		0x90 					 
#define ds1302_clkburst_add		0xbe

extern unsigned char time_buf1[8];//�Ŧ~���ɤ���P
extern unsigned char time_buf[8] ;//�Ŧ~���ɤ���P
/*------------------------------------------------
           �VDS1302�g�J�@�줸�ռƾ�
------------------------------------------------*/
void Ds1302_Write_Byte(unsigned char addr, unsigned char d);
/*------------------------------------------------
           �qDS1302Ū�X�@�줸�ռƾ�
------------------------------------------------*/
unsigned char Ds1302_Read_Byte(unsigned char addr) ;
/*------------------------------------------------
           �VDS1302�g�J�����ƾ�
------------------------------------------------*/
void Ds1302_Write_Time(void) ;
/*------------------------------------------------
           �qDS1302Ū�X�����ƾ�
------------------------------------------------*/
void Ds1302_Read_Time(void)  ;
/*------------------------------------------------
                DS1302��l��
------------------------------------------------*/
void Ds1302_Init(void);

#endif
