#include "tm4c123gh6pm.h"
#include "TM4C123.h"
#include "LCD.h"

typedef char * string;

/*The LCD initializes 
the ports A5-6-7 as the control pins  for LCD and portB0-7 
as the data pins*/
void LCD_INIT(void) {
		SET(SYSCTL_RCGCGPIO_R,1);
		while((SYSCTL_PRGPIO_R& (0x01 << 1)) == 0){}
    
    GPIO_PORTB_AFSEL_R &= ~0xff;    //disable alternative functions for portB
    GPIO_PORTB_AMSEL_R &= ~0Xff;    //disable analogue function
    GPIO_PORTB_PCTL_R &= ~0XFF;     //regular digital pins
    GPIO_PORTB_DIR_R  |= 0XFF;      //set the direction of PB0-7 as output
    GPIO_PORTB_DEN_R  |= 0XFF;      //enable digital portB

	
		// PA5 >> r/w, PA6 >> EN, PA7 >> RS
    SET(SYSCTL_RCGCGPIO_R,0);   // allow the clock for PA5,6,7
    while((SYSCTL_PRGPIO_R& (0x01)) == 0){}
    GPIO_PORTA_AFSEL_R &= ~0xE0;    //disable alternative functions for PA5,6,7
    GPIO_PORTA_AMSEL_R &= ~0XE0;    //disable analogue function for PA5,6,7
    GPIO_PORTA_PCTL_R &= ~0XE0;     //regular digital pins
    GPIO_PORTA_DIR_R |= 0XE4;       //set the direction of PA5,6,7 as output
    GPIO_PORTA_DEN_R |= 0XE4;       //enable digital PA5,6,7
}

void LCD_CMD(unsigned long cmd) {
    GPIO_PORTB_DATA_R = cmd;    //set PB7-0 as the passed command to the function
    LCD_RS = 0x00;  						//set PA7 register select pin to low
    LCD_RW = 0x00;  						//set PA5 r/w pin to low
    LCD_EN = 0x40;  						//set enable pin to high
    delay_micro(40);        						//short delay 
    LCD_EN = 0x00;  						//set enable pin to low 
}

void LCD_WRITE (char data ) {
    GPIO_PORTB_DATA_R = data;  //write the data to PB7-0
    LCD_RS = 0x80;  					 //set PA7 to high
    LCD_RW = 0x00;  					 //set pA5 to low
    LCD_EN = 0x40;  					 //set the enable pin high
    delay_micro(40);       					 //short delay 
    LCD_EN = 0x00;  					 //set the enable pin to low
}

void SysTick_Wait_Timer(int delay)
{
	NVIC_ST_CTRL_R = 0; //disable Timer
	NVIC_ST_RELOAD_R = delay- 1; 				 //to make 0.1 secound as our Tiva has clk = 16 MHZ
	NVIC_ST_CURRENT_R = 0; 													 //to clear counter value and underflow flag of counter
	NVIC_ST_CTRL_R |= 0x5; 	
	while((NVIC_ST_CTRL_R&0x00010000)==0) {
        ;                          /* wait for COUNT flag (bit 16) */
    }
}												 //to put at source clk 1 to get PROCESOR CLK NOT its 8th only and enable Timer
void delay_milli(int i)
{
 SysTick_Wait_Timer(i*16129);
}
void delay_micro(int i)
{
 SysTick_Wait_Timer(i*16);
}

void LCD_String(char* str)    
{
    int i;
    for(i=0;str[i]!=0;i++)  
    {
        LCD_WRITE(str[i]);  
    }
}
