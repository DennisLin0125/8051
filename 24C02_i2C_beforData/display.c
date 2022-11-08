#include"display.h"
#include"delay.h"

#define DataPort P0 //�w�q�ƾڰ� �{�����J��DataPort �h��P0 ����
//sbit LATCH1=P2^0;//�w�q��s�ϯ�� �q��s
//sbit LATCH2=P2^3;//                 ����s

unsigned char code dofly_DuanMa[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};// ��ܬq�X��0~9
unsigned char code dofly_WeiMa[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//���O�����������ƽX���I�G,�Y��X
unsigned char TempData[8]; //�x�s��ܭȪ�������ܼ�


/*------------------------------------------------
 ��ܨ禡�A�Ω�ʺA���˼ƽX��
 ��J�Ѽ� FirstBit ��ܻݭn��ܪ��Ĥ@��A�p���2��ܱq�ĤT�ӼƽX�޶}�l���
 �p��J0��ܱq�Ĥ@����ܡC
 Num��ܻݭn��ܪ���ơA�p�ݭn���99���ƭȫh�ӭȿ�J2
------------------------------------------------*/
void Display(unsigned char FirstBit,unsigned char Num)
{
      static unsigned char i=0;
	  

	   DataPort=0;   //�M�żƾڡA���������v
       LATCH1=1;     //�q��s
       LATCH1=0;

       DataPort=dofly_WeiMa[i+FirstBit]; //����X 
       LATCH2=1;     //����s
       LATCH2=0;

       DataPort=TempData[i]; //����ܼƾڡA�q�X
       LATCH1=1;     //�q��s
       LATCH1=0;
       
	   i++;
       if(i==Num)
	      i=0;


}
/*------------------------------------------------
                    �w�ɾ���l�Ƥl�{��
------------------------------------------------*/
void Init_Timer0(void)
{
 TMOD |= 0x01;	  //�ϥμҦ�1�A16��w�ɾ��A�ϥ�"|"�Ÿ��i�H�b�ϥΦh�өw�ɾ��ɤ����v�T		     
 //TH0=0x00;	      //���w���
 //TL0=0x00;
 EA=1;            //�`���_���}
 ET0=1;           //�w�ɾ����_���}
 TR0=1;           //�w�ɾ��}�����}
}
/*------------------------------------------------
                 �w�ɾ����_�l�{��
------------------------------------------------*/
void Timer0_isr(void) interrupt 1 
{
 TH0=(65536-2000)/256;		  //���s��� 2ms
 TL0=(65536-2000)%256;
 
 Display(0,8);

}

