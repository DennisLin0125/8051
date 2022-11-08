#include <reg52.h>

#define uchar unsigned char
#define uint unsigned int
	
#define SET_5X7_8DATA 0x38   // 0011 1000
#define SET_CLEAR 0x01   // 0000 0001
#define SET_CURSOR_1ROW 0x80   // 1000 0000
#define SET_CURSOR_2ROW 0x40   // 0100 0000
#define DISPLAY_SHIFT_LEFT 0x18   // 0001 1000
#define DISPLAY_SHIFT_RIGHT 0x1c   // 0001 1100

/*----------------------------------------------
0000 1DCB     D:�}���  C:��ܥ���   B:���а{�{
-----------------------------------------------*/
#define SET_DISPLAY_FlASH 0x0c   // 0000 1100

/*---------------------------------------------
0000 01NS     
N:��}�۰ʩM���Ц۰�+1    
S:1 n=1���� n=0�k��   S:0 ����
-----------------------------------------------*/
#define SET_ADD_LEFT_RIGHT 0x06   // 0000 0110
	
sbit RS=P2^4;
sbit RW=P2^5;
sbit EN=P2^6;

void delay(uint z)
{
	uint x,y;
	for(x=z;x>0;x--)
		for(y=110;y>0;y--);
}

void write_command(uchar com)
{
	RS=0;
	EN=1;
	P0=com;
	delay(5);
	EN=0;
}

void write_data(uchar WRdata)
{
	RS=1;
	EN=1;
	P0=WRdata;
	delay(5);
	EN=0;
}

void init()
{
	EN=0;
	RW=0;
	write_command(SET_5X7_8DATA);
	write_command(SET_DISPLAY_FlASH);
	write_command(SET_ADD_LEFT_RIGHT);
	write_command(SET_CLEAR);
}
/*------------------------------------------------
              �g�J�r��禡
------------------------------------------------*/
void write_str(uchar x,uchar y,uchar *s)
{
	if (y == 0) 
	{     
		write_command(0x80 + x);     //��ܲĤ@��
	}
	else 
	{      
		write_command(0xC0 + x);      //��ܲĤG��
	}        
	while (*s) 
	{     
		write_data(*s);     
		s++;
		delay(20);
	}
}

void DisplayShiftLeft()
{
	uchar i;
	write_str(0x10,0,"I LIKE DENNIS");
	write_str(0x12,1,"WWW.DENNIS.COM");
	for(i=0;i<16;i++)
	{
		write_command(DISPLAY_SHIFT_LEFT);
		delay(500);
	}
}
void main()
{
	init();
	write_str(0,0,"I LIKE DENNIS");
	write_str(0,1,"WWW.DENNIS.COM");
	while(1)
	{
		delay(200);
		write_command(DISPLAY_SHIFT_LEFT);
		//write_command(DISPLAY_SHIFT_RIGHT);
	}
}