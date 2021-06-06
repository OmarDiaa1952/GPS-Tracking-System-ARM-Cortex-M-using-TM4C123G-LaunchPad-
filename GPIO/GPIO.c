#include "TM4C123GH6PM.h"
#include "GPIO.h"

void initPortF(void){
	SET(SYSCTL_RCGCGPIO_R,5);
	while((SYSCTL_PRGPIO_R& (0x01 << 5)) == 0){}
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog on PF
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) PCTL GPIO on PF4-0
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 in, PF3-1 out
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) disable alt funct on PF7-0
  GPIO_PORTF_PUR_R = 0x11;          // enable pull-up on PF0 and PF4
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital I/O on PF4-0
}
void initPortD(void){ 
	SET(SYSCTL_RCGCGPIO_R,3);         // 1) activate clock for Port D
  while ((SYSCTL_PRGPIO_R & (0x01 << 3)) == 0);	// 2) allow time for clock to start
  GPIO_PORTD_CR_R = 0xF;                 // 4) allow changes to PD3-0
  GPIO_PORTD_AMSEL_R = 0x00;              // 5) disable analog on PD
  GPIO_PORTD_PCTL_R = 0x00000000;         // 6) PCTL GPIO on PD7-0 disabled i won't use Aleternate function
  GPIO_PORTD_DIR_R = 0x0F;                // 7) PD3-0 as output
  GPIO_PORTD_AFSEL_R = 0x00;              // 8) disable alt funct on PD7-0
  GPIO_PORTD_DEN_R = 0x0F;          		  // 10) enable digital I/O on PD3-0
}

void initPortE(void){ 
	SET(SYSCTL_RCGCGPIO_R,5);         // 1) activate clock for Port D
  while ((SYSCTL_PRGPIO_R & (0x01 << 5)) == 0);	// 2) allow time for clock to start
	SET(GPIO_PORTE_DIR_R,2);
	SET(GPIO_PORTD_DEN_R,2);
}

void led(int color){
	GPIO_PORTF_DATA_R &= (~0x0E);
	GPIO_PORTF_DATA_R |= color;
}
