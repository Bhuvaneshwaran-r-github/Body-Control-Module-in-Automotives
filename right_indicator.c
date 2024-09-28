#include<lpc21xx.h>
#define RIGHT_INDICATOR 1<<17

typedef int u32;
typedef struct CAN2_MSG {
  u32 id;
  u32 rtr;
  u32 dlc;
  u32 byteA; 
  u32 byteB;
}CAN;	
	 
void can2_init(void);	 
void can2_receive(CAN*);
void delay_ms(unsigned int);
							   
int main()
{
  	CAN m1;
  	can2_init();
	IODIR0|=RIGHT_INDICATOR;
	IOSET0=RIGHT_INDICATOR;
  	while(1)
	{
		can2_receive(&m1);
		if(m1.id==0x331)
		{
			if(m1.rtr==0)
			{
				if(m1.byteB==1)//turn on indicator 		
					IOCLR0=RIGHT_INDICATOR; 
			 	else	
					IOSET0=RIGHT_INDICATOR; 	 
			}
		}
	}
}

void can2_init()
{
	PINSEL1|=0X00014000;
	VPBDIV=1;	//making the Pclk as 60MHz
	C2MOD=0X01;	//CAN mode reset //No operation performed
	AFMR=0X02;	//Disabling the acceptance filter
	C2BTR=0X001C001D;//125kbps
	C2MOD=0X00;	//CAN normal mode //Node send or receive data
}

void can2_receive(CAN *m1)
{					   
	while((C2GSR&1)==0);   
	m1->id=C2RID;	  
	m1->dlc=(C2RFS>>16)&0xf;
	m1->rtr=(C2RFS>>30)&0x1;
	if(m1->rtr==0)
	{
		m1->byteA=C2RDA;
		m1->byteB=C2RDB;	
	}				
	C2CMR=(1<<2);//RRB==>Release Receive Buffer
}

void delay_ms(unsigned int ms)
{
	T0PR=15000-1;
	T0TCR=0X01;
	while(T0TC<ms);
	T0TCR=0X03;
	T0TCR=0X00;
}


