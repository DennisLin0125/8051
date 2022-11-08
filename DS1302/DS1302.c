
#include "ds1302.h"

unsigned char time_buf1[8] = {20,10,6,5,12,55,00,6};//�Ŧ~���ɤ���P
unsigned char time_buf[8] ;                         //�Ŧ~���ɤ���P
/*------------------------------------------------
           �VDS1302�g�J�@�줸�ռƾ�
------------------------------------------------*/
void Ds1302_Write_Byte(unsigned char addr, unsigned char d)
{

	unsigned char i;
	RST_SET;	

	//�g�J�ؼЦa�}�Gaddr
	addr = addr & 0xFE;     //�̧C��m�s
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

	//�g�J�ƾڡGd
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
	RST_CLR;					//����DS1302�׬y��
}
/*------------------------------------------------
           �qDS1302Ū�X�@�줸�ռƾ�
------------------------------------------------*/

unsigned char Ds1302_Read_Byte(unsigned char addr) 
{

	unsigned char i;
	unsigned char temp;
	RST_SET;	

	//�g�J�ؼЦa�}�Gaddr
	addr = addr | 0x01;//�̧C��m��
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

	//��X�ƾڡGtemp
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

	RST_CLR;	//����DS1302�׬y��
	return temp;
}

/*------------------------------------------------
           �VDS1302�g�J�����ƾ�
------------------------------------------------*/
void Ds1302_Write_Time(void) 
{
     
	unsigned char i,tmp;
	for(i=0;i<8;i++)
	{                  //BCD�B�z
		tmp=time_buf1[i]/10;
		time_buf[i]=time_buf1[i]%10;
		time_buf[i]=time_buf[i]+tmp*16;
	}
	Ds1302_Write_Byte(ds1302_control_add,0x00);			//�������g 
	Ds1302_Write_Byte(ds1302_sec_add,0x80);				//�Ȱ� 
	//Ds1302_Write_Byte(ds1302_charger_add,0xa9);			//���y�R�q 
	Ds1302_Write_Byte(ds1302_year_add,time_buf[1]);		//�~ 
	Ds1302_Write_Byte(ds1302_month_add,time_buf[2]);	//�� 
	Ds1302_Write_Byte(ds1302_date_add,time_buf[3]);		//�� 
	Ds1302_Write_Byte(ds1302_day_add,time_buf[7]);		//�P 
	Ds1302_Write_Byte(ds1302_hr_add,time_buf[4]);		//�� 
	Ds1302_Write_Byte(ds1302_min_add,time_buf[5]);		//��
	Ds1302_Write_Byte(ds1302_sec_add,time_buf[6]);		//��
	Ds1302_Write_Byte(ds1302_day_add,time_buf[7]);		//�P 
	Ds1302_Write_Byte(ds1302_control_add,0x80);			//���}���g 
}

/*------------------------------------------------
           �qDS1302Ū�X�����ƾ�
------------------------------------------------*/
void Ds1302_Read_Time(void)  
{ 
	unsigned char i,tmp;
	time_buf[1]=Ds1302_Read_Byte(ds1302_year_add);		//�~ 
	time_buf[2]=Ds1302_Read_Byte(ds1302_month_add);		//�� 
	time_buf[3]=Ds1302_Read_Byte(ds1302_date_add);		//�� 
	time_buf[4]=Ds1302_Read_Byte(ds1302_hr_add);		//�� 
	time_buf[5]=Ds1302_Read_Byte(ds1302_min_add);		//�� 
	time_buf[6]=(Ds1302_Read_Byte(ds1302_sec_add))&0x7F;//�� 
	time_buf[7]=Ds1302_Read_Byte(ds1302_day_add);		//�P 


	for(i=0;i<8;i++)
	{           //BCD�B�z
		tmp=time_buf[i]/16;
		time_buf1[i]=time_buf[i]%16;
		time_buf1[i]=time_buf1[i]+tmp*10;
	}
}

/*------------------------------------------------
                DS1302��l��
------------------------------------------------*/
void Ds1302_Init(void)
{
	
	RST_CLR;			//RST�}�m�C
	SCK_CLR;			//SCK�}�m�C
	Ds1302_Write_Byte(ds1302_sec_add,0x00);				 
}
