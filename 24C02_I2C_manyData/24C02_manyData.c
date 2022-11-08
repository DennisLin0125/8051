/*-----------------------------------------------
  �W�١GIIC��ĳ EEPROM24c02
  �s�g�Gdennid
  �ק�G�L
  ���e�G���{���Ω��˴�EEPROM�į�A���դ�k�p�U�G�g�J24c02�@�ǼƾڡA�M��b�O���餤�M���o�ǼƾڡA
        ���q��D�O����N���h�o�Ǹ�T�A�M��q24c02���դJ�o�ǼƾڡC�ݬO�_�P�g�J���ۦP�C
		  �禡�O�ĥγn�驵�ɪ���k����SCL�߽�,�T�ﰪ�����W�v�n�@ �@�w���ק�....(���ҬO1us����
		  �g��,�Y�����W�v�n�p��12MHZ)
------------------------------------------------*/  
  
              
#include <reg52.h>          //���Y�ɮת��]�t
#include <intrins.h>

#define  _Nop()  _nop_()        //�w�q�ū��O

// �`,�ܼƩw�q��
unsigned char code dofly_DuanMa[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,
		                  	         0x77,0x7c,0x39,0x5e,0x79,0x71};// ��ܬq�X��0~F
unsigned char code dofly_WeiMa[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//���O�����������ƽX���I�G,�Y��X

sbit LATCH1=P2^2;
sbit LATCH2=P2^3;
                                              
sbit SDA=P2^1;            //����I2C�ƾڶǰe��
sbit SCL=P2^0;            //����I2C���������

                          
bit ack;	              //�����лx��
   

void DelayUs2x(unsigned char t);//�禡�n�� 
void DelayMs(unsigned char t);

/*------------------------------------------------
 uS���ɨ禡�A�t����J�Ѽ� unsigned char t�A�L��^��
 unsigned char �O�w�q�L�Ÿ��r���ܼơA��Ȫ��d��O
 0~255 �o�̨ϥδ���12M�A��T���ɽШϥζ׽s,�j�P����
 ���צp�U T=tx2+5 uS 
------------------------------------------------*/
void DelayUs2x(unsigned char t)
{   
	while(--t);
}
/*------------------------------------------------
 mS���ɨ禡�A�t����J�Ѽ� unsigned char t�A�L��^��
 unsigned char �O�w�q�L�Ÿ��r���ܼơA��Ȫ��d��O
 0~255 �o�̨ϥδ���12M�A��T���ɽШϥζ׽s
------------------------------------------------*/
void DelayMs(unsigned char t)
{
     
	while(t--)
	{
		//�j�P����1mS
		DelayUs2x(245);
		DelayUs2x(245);
	}
}

/*------------------------------------------------
                    �Ұʶ׬y��
------------------------------------------------*/
void Start_I2c()
{
	SDA=1;   //�o�e�_�l���󪺼ƾڰT��
	_Nop();
	SCL=1;
	_Nop();    //�_�l����إ߮ɶ��j��4.7us,����
	_Nop();
	_Nop();
	_Nop();
	_Nop();    
	SDA=0;     //�o�e�_�l�T��
	_Nop();    //�_�l������w�ɶ��j��4�g
	_Nop();
	_Nop();
	_Nop();
	_Nop();       
	SCL=0;    //�X��I2C�׬y�ơA�ǳƵo�e�α����ƾ�
	_Nop();
	_Nop();
}
/*------------------------------------------------
                    �����׬y��
------------------------------------------------*/
void Stop_I2c()
{
	SDA=0;    //�o�e�������󪺼ƾڰT��
	_Nop();   //�o�e�������󪺮����T��
	SCL=1;    //��������إ߮ɶ��j��4�g
	_Nop();
	_Nop();
	_Nop();
	_Nop();
	_Nop();
	SDA=1;    //�o�eI2C�׬y�Ƶ����T��
	_Nop();
	_Nop();
	_Nop();
	_Nop();
}




/*----------------------------------------------------------------
                 �줸�ռƾڶǰe�禡               
�禡�쫬: void  SendByte(unsigned char c);
�\��:  �N�ƾ�c�o�e�X�h,�i�H�O�a�},�]�i�H�O�ƾ�,�o���Z��������,�ù�
     �����A��i��ާ@.(�������ΫD��������ack=0 ��)     
     �o�e�ƾڥ��`�Aack=1; ack=0��ܳQ�����L�����ηl�a�C
------------------------------------------------------------------*/
void  SendByte(unsigned char c)
{
	unsigned char BitCnt;

	for(BitCnt=0;BitCnt<8;BitCnt++)  //�n�ǰe���ƾڪ��׬�8��
	{
		if((c<<BitCnt)&0x80)
			SDA=1;   //�P�_�o�e��
		else  
			SDA=0; 
		
		_Nop();
		SCL=1;               //�m�����u�����A�q���Q�����}�l�����ƾڦ�
		_Nop(); 
		_Nop();             //�O�Ү������q���g���j��4�g
		_Nop();
		_Nop();
		_Nop();         
		SCL=0; 
	}

	_Nop();
	_Nop();
	SDA=1;               //8��o�e���Z����ƾڽu�A�ǳƱ���������
	_Nop();
	_Nop();   
	SCL=1;
	_Nop();
	_Nop();
	_Nop();
	if(SDA==1) //�P�_�O�_�����������T��
		ack=0;     
	else 
		ack=1;
	
	SCL=0;
	_Nop();
	_Nop();
}







/*----------------------------------------------------------------
                 �줸�ռƾڶǰe�禡               
�禡�쫬: unsigned char  RcvByte();
�\��:  �Ψӱ����q����ǨӪ��ƾ�,�çP�_�׬y�ƿ��~(���o�����T��)�A
     �o���Z�Х������禡�C  
------------------------------------------------------------------*/	
unsigned char  RcvByte()
{
	unsigned char retc;
	unsigned char BitCnt;

	retc=0; 
	SDA=1;             //�m�ƾڽu����J�覡
	for(BitCnt=0;BitCnt<8;BitCnt++)
	{
		_Nop();           
		SCL=0;       //�m�����u���C�A�ǳƱ����ƾڦ�
		_Nop();
		_Nop();      //�����C�q���g���j��4.7us
		_Nop();
		_Nop();
		_Nop();
		SCL=1;       //�m�����u�����ϼƾڽu�W�ƾڦ���
		_Nop();
		_Nop();
		retc=retc<<1;
		if(SDA==1)
			retc=retc+1; //Ū�ƾڦ�,�������ƾڦ��Jretc��
		_Nop();
		_Nop(); 
	}
	SCL=0;    
	_Nop();
	_Nop();
	return(retc);
}



/*----------------------------------------------------------------
                     �����l�禡
�쫬:  void Ack_I2c(void);
 
----------------------------------------------------------------*/
void Ack_I2c(void)
{
  
	SDA=0;     
	_Nop();
	_Nop();
	_Nop();      
	SCL=1;
	_Nop();
	_Nop();              //�����C�q���g���j��4�g
	_Nop();
	_Nop();
	_Nop();  
	SCL=0;               //�M�����u�A�X��I2C�׬y�ƥH�K�~�򱵦�
	_Nop();
	_Nop();    
}
/*----------------------------------------------------------------
                     �D�����l�禡
�쫬:  void NoAck_I2c(void);
 
----------------------------------------------------------------*/
void NoAck_I2c(void)
{
  
	SDA=1;
	_Nop();
	_Nop();
	_Nop();      
	SCL=1;
	_Nop();
	_Nop();              //�����C�q���g���j��4�g
	_Nop();
	_Nop();
	_Nop();  
	SCL=0;                //�M�����u�A�X��I2C�׬y�ƥH�K�~�򱵦�
	_Nop();
	_Nop();    
}






/*----------------------------------------------------------------
                    �V�L�l�a�}����o�e�줸�ռƾڨ禡               
�禡�쫬: bit  ISendByte(unsigned char sla,ucahr c);  
�\��:     �q�Ұʶ׬y�ƨ�o�e�a�}�A�ƾڡA�����׬y�ƪ����L�{,�q����a�}sla.
           �p�G��^1��ܾާ@���\�A�_�h�ާ@���~�C
�`�N�G    �ϥΫe�����w�����׬y�ơC
----------------------------------------------------------------*/
/*bit ISendByte(unsigned char sla,unsigned char c)
{
   Start_I2c();               //�Ұʶ׬y��
   SendByte(sla);             //�o�e����a�}
     if(ack==0)return(0);
   SendByte(c);               //�o�e�ƾ�
     if(ack==0)return(0);
  Stop_I2c();                 //�����׬y��
  return(1);
}
*/

/*----------------------------------------------------------------
                    �V���l�a�}����o�e�h�줸�ռƾڨ禡               
�禡�쫬: bit  ISendStr(unsigned char sla,unsigned char suba,ucahr *s,unsigned char no);  
�\��:     �q�Ұʶ׬y�ƨ�o�e�a�}�A�l�a�},�ƾڡA�����׬y�ƪ����L�{,�q����
          �a�}sla�A�l�a�}suba�A�o�e���e�Os���V�����e�A�o�eno�Ӧ줸�աC
           �p�G��^1��ܾާ@���\�A�_�h�ާ@���~�C
�`�N�G    �ϥΫe�����w�����׬y�ơC
----------------------------------------------------------------*/
bit ISendStr(unsigned char sla,unsigned char suba,unsigned char *s,unsigned char no)
{
	unsigned char i;

	Start_I2c();               //�Ұʶ׬y��
	SendByte(sla);             //�o�e����a�}
	if(ack==0)
		return(0);
	SendByte(suba);            //�o�e����l�a�}
	if(ack==0)
		return(0);

	for(i=0;i<no;i++)
	{   
		SendByte(*s);            //�o�e�ƾ�
		DelayMs(1);
		if(ack==0)return(0);
		s++;
	} 
	Stop_I2c();                  //�����׬y��
	return(1);
}

/*----------------------------------------------------------------
                    �V�L�l�a�}����Ū�줸�ռƾڨ禡               
�禡�쫬: bit  IRcvByte(unsigned char sla,ucahr *c);  
�\��:     �q�Ұʶ׬y�ƨ�o�e�a�}�AŪ�ƾڡA�����׬y�ƪ����L�{,�q����a
          �}sla�A��^�Ȧbc.
           �p�G��^1��ܾާ@���\�A�_�h�ާ@���~�C
�`�N�G    �ϥΫe�����w�����׬y�ơC
----------------------------------------------------------------*/
/*bit IRcvByte(unsigned char sla,unsigned char *c)
{
   Start_I2c();                //�Ұʶ׬y��
   SendByte(sla+1);            //�o�e����a�}
     if(ack==0)return(0);
   *c=RcvByte();               //Ū���ƾ�
     NoAck_I2c();              //�o�e�D�N����
     Stop_I2c();               //�����׬y��
  return(1);
}

*/
/*----------------------------------------------------------------
                    �V���l�a�}����Ū���h�줸�ռƾڨ禡               
�禡�쫬: bit  ISendStr(unsigned char sla,unsigned char suba,ucahr *s,unsigned char no);  
�\��:     �q�Ұʶ׬y�ƨ�o�e�a�}�A�l�a�},Ū�ƾڡA�����׬y�ƪ����L�{,�q����
          �a�}sla�A�l�a�}suba�AŪ�X�����e��Js���V���x�s�ϡAŪno�Ӧ줸�աC
           �p�G��^1��ܾާ@���\�A�_�h�ާ@���~�C
�`�N�G    �ϥΫe�����w�����׬y�ơC
----------------------------------------------------------------*/
bit IRcvStr(unsigned char sla,unsigned char suba,unsigned char *s,unsigned char no)
{
	unsigned char i;

	Start_I2c();               //�Ұʶ׬y��
	SendByte(sla);             //�o�e����a�}
	if(ack==0)
		return(0);
	SendByte(suba);            //�o�e����l�a�}
	if(ack==0)
		return(0);

	Start_I2c();
	SendByte(sla+1);
	if(ack==0)
		return(0);

	for(i=0;i<no-1;i++)
	{   
		*s=RcvByte();              //�o�e�ƾ�
		Ack_I2c();                //�o�e�N���� 
		s++;
	} 
	*s=RcvByte();
	NoAck_I2c();                 //�o�e�D����
	Stop_I2c();                    //�����׬y��
	return(1);
}
/*------------------------------------------------
                    �D�禡
------------------------------------------------*/
void main()
{
	unsigned char dofly[4]={5,6,7,8};          // ��ܽX�� 1234
	unsigned char i;

	ISendStr(0xae,4,dofly,4);                   //�g�J24c02
	DelayMs(200);
	dofly[0]=0;                                  //�M���ثe�ƾ�
	dofly[1]=0;
	dofly[2]=0;
	dofly[3]=0;
	IRcvStr(0xae,4,dofly,4);                  //�I�s�x�s�ƾ�

	while(1)
	{ 

		P0=dofly_DuanMa[dofly[i]];//����x�s�ƾ�
		LATCH1=1;                  //��s
		LATCH1=0;

		P0=dofly_WeiMa[i];  //����X
		LATCH2=1;          // ��s
		LATCH2=0;
		DelayMs(200);      //���ɥΩ�t����ܼƾ�
		DelayMs(200);
		i++;
		if(i==4)
			i=0;
	}
}



