#include "SYSTICK.h"
#include "TM4C123GH6PM.h"

// PortD intialization
void init_PortD(void){ 
  SYSCTL_RCGC2_R |= 0x00000008;           // 1) activate clock for Port D
  while ((SYSCTL_PRGPIO_R & 0x08) == 0);	// 2) allow time for clock to start
	
  GPIO_PORTD_CR_R = 0xF;                 // 4) allow changes to PD3-0
  GPIO_PORTD_AMSEL_R = 0x00;              // 5) disable analog on PD
  GPIO_PORTD_PCTL_R = 0x00000000;         // 6) PCTL GPIO on PD7-0 disabled i won't use Aleternate function
  GPIO_PORTD_DIR_R = 0x0F;                // 7) PD3-0 as output
  GPIO_PORTD_AFSEL_R = 0x00;              // 8) disable alt funct on PD7-0
  GPIO_PORTD_PUR_R = 0x00;                // 9) disable pull-up on PD3-0
  GPIO_PORTD_DEN_R = 0x0F;          		  // 10) enable digital I/O on PD3-0
}

//functions for counting
void pulse(void);
void countingDistance(double L);
void resetCounters(void);
double fractions = 0;
