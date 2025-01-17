#include<lpc21xx.h>
#include"lcd_fourbit.h" 

#define LEFT_IND_SW 14	 
#define RIGHT_IND_SW 15
#define MOTOR_CW 16	
#define MOTOR_ACW 12
#define MOTOR_IDLE 13
#define SWITCH_PRESSED(PIN) ((IOPIN0>>PIN)&1)==0
#define byteB_flag(TX,DATA) do{\
			TX=!TX;\
			if(TX==1)\
				m1.byteB=DATA;\
			else\
				m1.byteB=0;\
		}while(0)
			
typedef unsigned int u32;	
typedef unsigned char u8;
typedef struct CAN2_MSG {
  u32 id;
  u32 rtr;
  u32 dlc;
  u32 byteA; 
  u32 byteB;
}CAN;	
	 
void can2_init(void);	   
void can2_send(CAN);
void node(CAN,u32,u32,u8*,u8);
  
int main()
{   
  u32 TX1=0,TX2=0,TX3=0,TX4=0,TX5=0;//Transmitting flags to check the ecu is in ON or OFF condition
  CAN m1;
  lcd_init();//Dashboard
  can2_init();
  m1.rtr=0;//data frame
  m1.dlc=8;//no.of bytes
  m1.byteA=0;					    
  lcd_str("BODY CONTROL MODULE IN AUTOMOTIVES"); 
  
  while(1)
  {
		if(SWITCH_PRESSED(LEFT_IND_SW))
			node(m1,0x221,TX1=!TX1,"LEFT INDICATOR",LEFT_IND_SW); //TX1=!TX1 means first time indicator on next time off 
		if(SWITCH_PRESSED(RIGHT_IND_SW))
			node(m1,0x331,TX2=!TX2,"RIGHT INDICATOR",RIGHT_IND_SW);  
		if(SWITCH_PRESSED(MOTOR_CW))
		{ 
			byteB_flag(TX3,1);//if TX3 =1 the send the data as 1(for clockwise)
			node(m1,0x293,m1.byteB,"MOTOR CLOCKWISE",MOTOR_CW);  
		}
		if(SWITCH_PRESSED(MOTOR_ACW))
		{
			byteB_flag(TX4,2);//if TX4 =1 the send the data as 2(for Anti-clockwise)
			node(m1,0x293,m1.byteB,"MOTOR ANTICLOCKWISE",MOTOR_ACW); 
		}
		if(SWITCH_PRESSED(MOTOR_IDLE))
		{
			byteB_flag(TX5,3);//if TX5 =1 the send the data as 3(for Idle position)			
			node(m1,0x293,m1.byteB,"MOTOR IDLE",MOTOR_IDLE); 
		}  	
	}
} 


void node(CAN m1, u32 ID, u32 data, u8*str, u8 OPERATION)
{	 	   
   	lcd_command(0X01);
   	lcd_command(0X80);
   	lcd_str(str);
 	m1.id=ID;
	m1.byteB=data;
	can2_send(m1);
	delay_ms(200);//To avoid switch bouncing
	while(SWITCH_PRESSED(OPERATION));
}  


void can2_init()
{
	PINSEL1|=0X00014000;//P0.23 & P0.24
	VPBDIV=1;	//making the Pclk as 60MHz
	C2MOD=0X01;	//CAN mode reset //No operation performed
	AFMR=0X02;	//Disabling the acceptance filter
	C2BTR=0X001C001D;//125kbps
	C2MOD=0X00;	//CAN normal mode //Node send or receive data
}

void can2_send(CAN m1)
{
 	C2TID1=m1.id;
	C2TFI1=(m1.dlc<<16);	
	if(m1.rtr==0) //if data frame
	{
		C2TFI1&=~(1<<30);//RTR=0
		C2TDA1=m1.byteA; //lower 4bytes of data
		C2TDB1=m1.byteB; //upper 4bytes of data
	}
	else
		C2TFI1|=(1<<30); //RTR=1
	C2CMR=(1<<0)|(1<<5);//Start Xmission & select Tx Buf1
	while((C2GSR&(1<<3))==0);//wait for data Xmission.
}


