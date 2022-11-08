#include <reg52.h>
#include "I2C.h"

sbit SDA=P2^0;
sbit SCL=P2^1;

void delay()
{ ; ; }
void START()
{
	SDA=1;
	delay();
	SCL=1;
	delay();
	SDA=0;
	delay();
}
void STOP()
{
	SDA=0;
	delay();
	SCL=1;
	delay();
	SDA=1;
	delay();
}
void response()
{
	uchar i;
	SCL=1;
	delay();
	while ((SDA==1)&&(i<250))
		i++;
   SCL=0;
	delay();	
}
void init()
{
	SDA=1;
	SCL=1;
}

void write_byte(uchar mydata)
{
	uchar i,temp;
	temp=mydata;
	SCL=0;
	delay();
	for(i=0;i<8;i++)
	{
		temp<<=1;
		SDA=CY;
		delay();
		SCL=1;
		delay();
		SCL=0;
		delay();
	}
	SDA=1;
	delay();
}
uchar read_byte()
{
	uchar i,temp,RDdata;
	SCL=0;
	delay();
	SDA=1;
	delay();
	for(i=0;i<8;i++)
	{
		SCL=1;
		delay();
		temp=SDA;
		RDdata=(RDdata<<1)|temp;
		SCL=0;
		delay();
	}
	return RDdata;
}
void delay1(uchar x)
{
	uchar a,b;
	for(a=x;a>0;a--)
		for(b=100;b>0;b--);
}
void WriteDataToAddress(uchar WRaddress,uchar WRdata)
{
	START();
	write_byte(DeviceAddressWR);
	response();
	write_byte(WRaddress);
	response();
	write_byte(WRdata);
	response();
	STOP();
}
uchar RandomReadFromAddress(uchar RDaddress)
{
	uchar RDdata;
	START();
	write_byte(DeviceAddressWR);
	response();
	write_byte(RDaddress);
	response();
	START();
	write_byte(DeviceAddressRD);
	response();
	RDdata=read_byte();
	STOP();
	return RDdata;
}

