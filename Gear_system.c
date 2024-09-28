#include<lpc21xx.h>
#define A1 (1<<5) //L293D driver input1(1A)			   
#define A2 (1<<6) //L293D driver input2(2A)
#define BUZZER 1<<7


typedef int u32;
typedef struct CAN2_MSG {
  u32 id;
  u32 rtr;
  u32 dlc;
  u32 byteA; 
  u32 byteB;
}CAN;	
	 
void can2_init(void);	   
void can2_send(CAN);   
void can2_receive(CAN*);
void delay_ms(unsigned int);

						   
int main()
{
	char prev=0;
  CAN m1;
  can2_init();
	IODIR0|=A1|A2|LED;//LED P0.17
	IOSET0=A1|A2|LED; //output direction for motor driver//Motor to idle position
  while(1)
	{
		can2_receive(&m1);
		if(m1.id==0x293)
		{
			if(m1.byteB==1)//Turn ON motor(Clockwise) (GEAR REVERSE)
				received(m1,1,prev,A1,A2);//Clockwise direction
			if(m1.byteB==2)//Turn ON motor(Anti-Clockwise) (GEAR FORWARD)
				received(m1,2,prev,A2,A1);//Anti-Clockwise direction
			if(m1.byteB==3)		//Turn ON motor(Idle)	
			{
				prev=0;
				IOSET0=A1|A2;			
			}
		}
	}
}


void received(CAN m1,u32 data,char*prev,char a1,char a2)
{
	if(m1.byteB==data)//Turn ON motor(Clockwise) (GEAR REVERSE)
				{	
					if(*prev==0)//If MOTOR in idle state
					{
						*prev=1;				
						IOCLR0=a1;		
						IOSET0=a2; 
					}
					else//If MOTOR already runs in Anti-clockwise warn 
					{
						IOCLR0=BUZZER;
						delay_ms(500);
						IOSET0=BUZZER;
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


/*
int main()
{
	char prev=0;
  CAN m1;
  can2_init();
	IODIR0|=A1|A2|LED;//LED P0.17
	IOSET0=A1|A2|LED; //output direction for motor driver//Motor to idle position
  while(1)
	{
		can2_receive(&m1);
		if(m1.id==0x293)
		{
			if(m1.byteB==1)//Turn ON motor(Clockwise) (GEAR REVERSE)
			{	
				if(prev==0)//If MOTOR in idle state
				{
					prev=1;				
					IOCLR0=A1;		
					IOSET0=A2; 
		 		}
				else//If MOTOR already runs in Anti-clockwise warn 
				{
					IOCLR0=BUZZER;
					delay_ms(500);
					IOSET0=BUZZER;
				}
			}
			if(m1.byteB==2)//Turn ON motor(Anti-Clockwise) (GEAR FORWARD)
			{		
				if(prev==0)//If MOTOR in idle state, then only the engine starts
				{
					prev=1;		
					IOCLR0=A2;  		
					IOSET0=A1;	
				}			   
				else//If MOTOR already runs in clockwise warn
				{
					IOCLR0=BUZZER;
					delay_ms(500);
					IOSET0=BUZZER;
				}
			}
			if(m1.byteB==3)		//Turn ON motor(Idle)	
			{
				prev=0;
				IOSET0=A1|A2;			
			}
		}
	}
}
*/


