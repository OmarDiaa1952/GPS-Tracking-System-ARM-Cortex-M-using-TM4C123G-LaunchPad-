#include "SYSTICK.h"
#include "TM4C123GH6PM"

void init_Timer_delay(void) //intialization of SysTick Counter Timer
{
	NVIC_ST_CTRL_R = NVIC_ST_CTRL_R & ~(0x00000001); //disable Timer
	NVIC_ST_RELOAD_R = 16000000 - 1; 		 //to make 1 secound as our Tiva has clk = 16 MHZ
	NVIC_ST_CURRENT_R = 0; 			         //to clear counter value and underflow flag of counter
	NVIC_ST_CTRL_R |= 0x5; 				 //to put at source clk 1 to get PROCESOR CLK NOT its 8th only and enable Timer
}

void hundredMicroSecounds(void)
{
	NVIC_ST_CTRL_R = NVIC_ST_CTRL_R & ~(0x00000001); //disable Timer
	NVIC_ST_RELOAD_R = (1600000 - 1); 		//to make 1 secound as our Tiva has clk = 16 MHZ
	NVIC_ST_CURRENT_R = 0; 				//to clear counter value and underflow flag of counter
	NVIC_ST_CTRL_R |= 0x5; 				//to put at source clk 1 to get PROCESOR CLK NOT its 8th only and enable Timer
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
		else if (s == 0)  //2nd base case
		{
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
	int i;
	for (i = 0; i < frac; i++)
	{
		hundredMicroSecounds();
		while ((NVIC_ST_CTRL_R & 0x00010000) == 0) {}
	}
	return;
}
