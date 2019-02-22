//Function: This procedure applies to read DHT11 humidity temperature value. H: humidity, T: temperature, V: checksum
//Platform£ºATMEGA64
//Fuse bytes£º
//		  Low byte£º0xc4
//		  High byte£º0xd9
//		  Extend£º0xFF
//		  Lock byte£º0xff;
//Crystal : Internal RC oscillator 8MHz
//Time: August 6, 2012

#include <avr/io.h>
typedef unsigned char U8;           
typedef unsigned int   U16;
#define uint unsigned int 
#define uchar unsigned char   

U8 U8count,U8temp;
U8 U8T_data_H,U8T_data_L,U8RH_data_H,U8RH_data_L,U8checkdata;
U8 U8T_data_H_temp,U8T_data_L_temp,U8RH_data_H_temp,U8RH_data_L_temp,U8checkdata_temp;
U8 U8FLAG,U8comdata;

uchar String_R[]="H:   .   %RH    ";
uchar String_T[]="T:   .   C V:   ";
uchar disp_tab[]={'0','1','2','3','4','5','6','7','8','9'};
uint count=0;
#define	    C4_IN	  DDRC &= ~(1 << PC0)		//Set the input
#define	    C4_OUT	  DDRC |= (1 << PC0)		//Set the output
#define	    C4_CLR	  PORTC &= ~(1 << PC0)	    //Set low
#define	    C4_SET	  PORTC |= (1 << PC0)		//Set high
#define	    C4_R	  (PINC & (1 <<PC0))		//Reading level

#define		COM_IN          C4_IN  
#define	    COM_OUT         C4_OUT
#define     COM_CLR         C4_CLR
#define     COM_SET         C4_SET
#define     COM_R           C4_R
void Delay(uchar x)//100.13us * x
{     
	uchar i,j;
	for(j=x;j>0;j--)
		for(i=74;i>0;i--);
}
void delay1(uint x)//101.25us * x
{
	uint i,j;
	for(i=x;i>0;i--)
		for(j=40;j>0;j--);
}
void Delay_19us(uchar x)//8.75us * x
{
	 while(x--)asm("nop");

} 
void Write_Command(uchar command)//Write instruction of LCD1602
{
	PORTD &=~(1<<PD2);//E=0;
	PORTD &=~(1<<PD0);//RS=0;
	PORTD &=~(1<<PD1);//RW=0;	
	PORTA = command;
	PORTD |=(1<<PD2);//E=1;	
	delay1(10);
	PORTD &=~(1<<PD2);//E=0;
}
void Write_Data(uchar dat)//Write data of LCD1602
{
	PORTD &=~(1<<PD2);//E=0;
	PORTD |=(1<<PD0);//RS=1;
	PORTD &=~(1<<PD1);//RW=0;
	PORTA = dat;
	PORTD |=(1<<PD2);//E=1;
	delay1(10);
	PORTD &=~(1<<PD2);//E=0;
}
void init(void)//LCD1602 Initialization
{
	delay1(150);
	Write_Command(0x38);
	delay1(50);
	Write_Command(0x38);
	delay1(50);
	Write_Command(0x38);
	Write_Command(0x38);
	Write_Command(0x08);
	Write_Command(0x01);
	Write_Command(0x06);
	Write_Command(0x0c);		
}
void displayonechar(uchar command,uchar dat)//The specified location in the LCD1602 display
{
	Write_Command(command);
	Write_Data(dat);
}

void show(void)
{
	displayonechar(0x80+0x02,disp_tab[U8RH_data_H/100]);//Humidity value of the integer part of the one hundred
	displayonechar(0x80+0x03,disp_tab[(U8RH_data_H%100)/10]);//Humidity value of the integer part of the ten
	displayonechar(0x80+0x04,disp_tab[U8RH_data_H%10]);//Bits of the integer part of the humidity values
	displayonechar(0x80+0x06,disp_tab[U8RH_data_L/100]);//Humidity values ??of the fractional part of one hundred
	displayonechar(0x80+0x07,disp_tab[(U8RH_data_L%100)/10]);//Humidity values ??of the fractional part of ten
	displayonechar(0x80+0x08,disp_tab[U8RH_data_L%10]);//Bits of the fractional part of the humidity values
	
	displayonechar(0x80+0x40+0x02,disp_tab[U8T_data_H/100]);//Integer part of the temperature of one hundred
	displayonechar(0x80+0x40+0x03,disp_tab[(U8T_data_H%100)/10]);//Temperature value of the integer part of the ten
	displayonechar(0x80+0x40+0x04,disp_tab[U8T_data_H%10]);//Bits of the integer part of the temperature
	displayonechar(0x80+0x40+0x06,disp_tab[U8T_data_L/100]);//Temperature value of the fractional part of one hundred
	displayonechar(0x80+0x40+0x07,disp_tab[(U8T_data_L%100)/10]);//Temperature value of the fractional part of ten
	displayonechar(0x80+0x40+0x08,disp_tab[U8T_data_L%10]);//Bits of the fractional part of the temperature

	displayonechar(0x80+0x40+0x0d,disp_tab[U8checkdata/100]);//Checksum value of one hundred
	displayonechar(0x80+0x40+0x0e,disp_tab[(U8checkdata%100)/10]);//Checksum value of ten
	displayonechar(0x80+0x40+0x0f,disp_tab[U8checkdata%10]);//Bits of the checksum value
}
void COM(void)//The read DHT11 40 bits
{    
	U8 i;
	for(i=0;i<8;i++)    
	{				
		U8FLAG=2;
		while((!(COM_R))&&U8FLAG++);//50us low, waiting for high appear
									//delay26-28us£¬0.63+8.75*3+0.125*12+0.25=28.63us
									//Judge to receive the high and low level, high receiver '1 ', otherwise '0'
		if(U8FLAG==1)break;//0.63us
		Delay_19us(1);//8.75us*3
		Delay_19us(1);
		Delay_19us(1);
		asm("nop");asm("nop");asm("nop");asm("nop");//0.125us
		asm("nop");asm("nop");asm("nop");asm("nop");
		asm("nop");asm("nop");asm("nop");asm("nop");
		U8temp=0;//0.25us
		if(COM_R) U8temp=1;//Judge to receive high or low level after a delay for high U8temp = 1 or U8temp unchanged
		U8FLAG=2;
		while((COM_R)&&U8FLAG++);//Waiting for the high end
		U8comdata <<= 1;
		U8comdata |= U8temp;//The received data is stored in U8comdata the lowest level
	}
}

//-----------------------------------------------------
//----- Humidity reading program ----------------------
//-----------------------------------------------------
//---- The following variables are global variables ---
//---- Temperature 8 high == U8T_data_H ---------------
//---- Low temperature 8 == U8T_data_L ----------------
//---- High humidity 8 == U8RH_data_H -----------------
//---- 8 == low humidity U8RH_data_L ------------------
//---- Check 8 == U8checkdata -------------------------
//---- Call the subroutine as follows -----------------
//---- Delay ();, Delay_19us ();, COM ();--------------
//-----------------------------------------------------

void RH(void)//Start the beginning and humidity reading
{

	COM_OUT;//Port output
	COM_CLR;//Host at least down 18us
	Delay(200);
	COM_IN;//Waiting for receiving DHT11 response signal
	COM_SET;//Bus pull-up resistor pulled host delay 20-40us
	Delay_19us(1);//8.75us*4=35us
	Delay_19us(1);
	Delay_19us(1);
	Delay_19us(1);
	if(COM_R)U8FLAG=1;//If this is high, there is no response
	while(!(COM_R))//To a DHT11 response signal
	{
		U8FLAG=2;
		while((!(COM_R))&&((U8FLAG++)));//Receive 80us low, waiting to receive a 80us high
		if(U8FLAG==1) break;//Timeout, out of the 'while' statement
		U8FLAG=2;
		while((COM_R)&&U8FLAG++);//Receive 80us low, waiting to receive the 40 value
		if(U8FLAG==1) break;

		COM();
		if(U8FLAG==1) break;	
		U8RH_data_H_temp=U8comdata; 
		COM();
		if(U8FLAG==1) break;
		U8RH_data_L_temp=U8comdata;									
		COM();
		if(U8FLAG==1) break;
		U8T_data_H_temp=U8comdata;
		COM();
		if(U8FLAG==1) break;
		U8T_data_L_temp=U8comdata;
		COM();
		if(U8FLAG==1) break;
		U8checkdata_temp=U8comdata;

		COM_IN;
		COM_SET;
		//Check
		U8temp=(U8T_data_H_temp+U8T_data_L_temp+U8RH_data_H_temp+U8RH_data_L_temp);
		if(U8temp==U8checkdata_temp)
		{
			U8RH_data_H=U8RH_data_H_temp;
			U8RH_data_L=U8RH_data_L_temp;
			U8T_data_H=U8T_data_H_temp;
			U8T_data_L=U8T_data_L_temp;
			U8checkdata=U8checkdata_temp;
		}
		COM_IN;
		COM_SET;
		break;
	}

}
int main(void)
{
	DDRA = 0Xff;
	DDRD =0Xff;
	init();

	Write_Command(0x80);//Display static data
	for(count=0;count<16;count++) Write_Data(String_R[count]);
	Write_Command(0x80+0x40);
	for(count=0;count<16;count++) Write_Data(String_T[count]);
	while(1)
	{
		RH();//Read DHT11
		show();//Display dynamic data
	}
}
