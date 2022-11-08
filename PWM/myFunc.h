	
#define  BIT0 (1<<0)
#define  BIT1 (1<<1)
#define  BIT2 (1<<2)
#define  BIT3 (1<<3)
#define  BIT4 (1<<4)
#define  BIT5 (1<<5)
#define  BIT6 (1<<6)
#define  BIT7 (1<<7)

#define SET(seg,b) reg |= (b)
#define CLR(reg,b) reg &= ~(b)
#define TGL(reg,b) reg ^= (b)

#define uchar unsigned char
#define uint unsigned int