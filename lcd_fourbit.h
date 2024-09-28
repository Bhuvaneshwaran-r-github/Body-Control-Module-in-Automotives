#define LCD_D 0x0f<<20
#define RS 1<<17
#define RW 1<<18
#define E 1<<19	 
			   
typedef unsigned char u8;

void lcd_init(void);
void lcd_command(u8);
void lcd_data(unsigned);
void delay_ms(unsigned int ms);
void lcd_str(u8*);
void lcd_integer(int);
void lcd_float(float);
void cgram_write(int); 
void lcd_animate(char, char);

void delay_ms(unsigned int ms)
{
	T0PR=60000-1;//for can 60MHz give 60000
	T0TCR=0X01;
	while(T0TC<ms);
	T0TCR=0X03;
	T0TCR=0X00;
}

void lcd_init(void)
{
   IODIR1|=RS|RW|E|LCD_D;
   IOCLR1=RW;
   lcd_command(0X01);
   lcd_command(0X02);
   lcd_command(0X0C);
   lcd_command(0X28);
   lcd_command(0X80);
}
void lcd_command(u8 cmd)
{
	IOCLR1=LCD_D;
	IOSET1=(cmd&0xf0)<<16;
	IOCLR1=RS;
	IOSET1=E;
	delay_ms(2);
	IOCLR1=E;
	
	IOCLR1=LCD_D;
	IOSET1=(cmd&0x0f)<<20;
	IOCLR1=RS;
	IOSET1=E;
	delay_ms(2);
	IOCLR1=E;
}
void lcd_data(unsigned d)
{
	IOCLR1=LCD_D;
	IOSET1=(d&0xf0)<<16;
	IOSET1=RS;
	IOSET1=E;
	delay_ms(2);
	IOCLR1=E;
	
	IOCLR1=LCD_D;
	IOSET1=(d&0x0f)<<20;
	IOSET1=RS;
	IOSET1=E;
	delay_ms(2);
	IOCLR1=E;
}
void lcd_str(u8 *str)
{	  
	int pos=0;
	while(*str)
	{
		lcd_data(*str++);
		if(pos==16)
			lcd_command(0xC0);
		pos++;
	}
}

void lcd_integer(int n)
{ 
	int i=0;
	unsigned int arr[5];
	
	if(n==0)
		lcd_data('0');
	else
	{
		if(n<0)
		{
		  lcd_data('-');
		  n=-n;
		}
		
		while(n>0)
		{
		  arr[i++]=n%10;
		  n=n/10;
		}
		for(--i;i>=0;i--)
		lcd_data(arr[i]+48);
	}
}

void lcd_float(float f)
{
	int temp;
	temp=f;
	lcd_integer(temp);
	lcd_data('.');
	temp=(f-temp)*1000;
	lcd_integer(temp);
}






