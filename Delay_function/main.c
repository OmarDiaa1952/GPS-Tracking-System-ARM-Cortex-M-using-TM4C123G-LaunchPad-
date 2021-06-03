// Port F
#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define GPIO_LOCK_KEY           0x4C4F434B  // Unlocks the GPIO_CR register


// Global Registers and Macro Functions for all Ports
#define SYSCTL_RCGC2_R				  (*((volatile unsigned long *)0x400FE608))
#define SYSCTL_PRGPIO_R			  	(*((volatile unsigned long *)0x400FEA08)) 	

// PortF intialization
void init_PortF(void){ 
  SYSCTL_RCGC2_R |= 0x00000020;           // 1) activate clock for Port F
	while ((SYSCTL_PRGPIO_R & 0x20) == 0);	// 2) allow time for clock to start
	
	// only PF0 needs to be unlocked, other bits can't be locked
  GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;      // 3) unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;                 // 4) allow changes to PF4-0
	
  GPIO_PORTF_AMSEL_R = 0x00;              // 5) disable analog on PF
  GPIO_PORTF_PCTL_R = 0x00000000;         // 6) PCTL GPIO on PF4-0
  GPIO_PORTF_DIR_R = 0x0E;                // 7) PF4,PF0 in, PF3-1 out
  GPIO_PORTF_AFSEL_R = 0x00;              // 8) disable alt funct on PF7-0
  GPIO_PORTF_PUR_R = 0x11;                // 9) enable pull-up on PF0 and PF4
  GPIO_PORTF_DEN_R = 0x1F;          		  // 10) enable digital I/O on PF4-0
}

#include "tm4c123gh6pm.h" // that has all timer files
void init_Timer_delay(void)
{
	NVIC_ST_CTRL_R = NVIC_ST_CTRL_R & ~(0x00000001); //disable Timer
	NVIC_ST_RELOAD_R = 16000000 - 1; //to make 1 secound as our Tiva has clk = 16 MHZ
	NVIC_ST_CURRENT_R = 0; //to clear counter value and underflow flag of counter
	NVIC_ST_CTRL_R |= 0x5; //to put at source clk 1 to get PROCESOR CLK NOT its 8th only and enable Timer
}
void delay(int s, int d);

unsigned int frac = 0;
unsigned int counter = 0;

int main()
{
init_PortF();


	while (1)
	{
		counter++;
		GPIO_PORTF_DATA_R &= ~(0x0E); //clear leds
		GPIO_PORTF_DATA_R |= 0x02;   //Read On
		delay(2, 3);
		GPIO_PORTF_DATA_R ^= 0x02;
	}
}

void delay(int s, int d)
{
	init_Timer_delay();
	if (s == 1) //base case of recursive
	{
		if ((NVIC_ST_RELOAD_R & 0x10000) == 1)
			;
		//fraction part
		NVIC_ST_CTRL_R = NVIC_ST_CTRL_R & ~(0x00000001);
		NVIC_ST_RELOAD_R = (d / 10) * (16000000 - 1);
		NVIC_ST_CURRENT_R = 0; 
		NVIC_ST_CTRL_R |= 0x5;
		if ((NVIC_ST_RELOAD_R & 0x10000) == 1)
		{
			return;
		}
	}
	else //recruisive body
	{
		if ((NVIC_ST_RELOAD_R & 0x10000) == 1)
		{
			delay(s-1, d);
		}
	}
}