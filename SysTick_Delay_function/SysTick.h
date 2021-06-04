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
void init_PortF(void);

#include "tm4c123gh6pm.h" // that has all timer files
void init_Timer_delay(void);
void delay(int s, int d);
void delay_fraction(int d);
