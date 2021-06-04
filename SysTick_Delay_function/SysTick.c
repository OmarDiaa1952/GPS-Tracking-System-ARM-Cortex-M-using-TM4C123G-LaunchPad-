#include "SysTick.h"

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

unsigned int counter = 0;  //as 2.3 secound pass counter increase and led toogle
unsigned int i = 0; 		   //for the for loop of hundred micro seconds

void init_Timer_delay(void) //intialization of SysTick Counter Timer
{
	NVIC_ST_CTRL_R = NVIC_ST_CTRL_R & ~(0x00000001); //disable Timer
	NVIC_ST_RELOAD_R = 16000000 - 1; 								 //to make 1 secound as our Tiva has clk = 16 MHZ
	NVIC_ST_CURRENT_R = 0; 													 //to clear counter value and underflow flag of counter
	NVIC_ST_CTRL_R |= 0x5; 													 //to put at source clk 1 to get PROCESOR CLK NOT its 8th only and enable Timer
}

void hundredMicroSecounds(void)
{
	NVIC_ST_CTRL_R = NVIC_ST_CTRL_R & ~(0x00000001); //disable Timer
	NVIC_ST_RELOAD_R = (1600000 - 1); //to make 1 secound as our Tiva has clk = 16 MHZ
	NVIC_ST_CURRENT_R = 0; //to clear counter value and underflow flag of counter
	NVIC_ST_CTRL_R |= 0x5; //to put at source clk 1 to get PROCESOR CLK NOT its 8th only and enable Timer
}

void delay(int s, int d) //recursive function of the delay
{
	init_Timer_delay();
		if (s == 1) //base case of recursive
		{
			while ((NVIC_ST_CTRL_R & 0x00010000) == 0) {}
			Delay_Of_Fractions(d);
			return;
		}
		else //recruisive body
		{
			while ((NVIC_ST_CTRL_R & 0x00010000) == 0) {}
			delay(s-1, d);
		}
}

void Delay_Of_Fractions(int frac)
{
	for (i = 0; i < frac; i++)
	{
		hundredMicroSecounds();
		while ((NVIC_ST_CTRL_R & 0x00010000) == 0) {}
	}
	return;
}

int main()
{
init_PortF();

GPIO_PORTF_DATA_R |= 0x02;   //Read On

	while (1)
	{
		delay(2, 1); //example to test delay function 2.1 secound then counter increase and led on
		counter++;
		GPIO_PORTF_DATA_R ^= 0x02;
	}
}
