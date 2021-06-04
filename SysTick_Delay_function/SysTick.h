#include "tm4c123gh6pm.h" // that has all timer files

// PortF intialization
void init_PortF(void);

// SysTick intialization
void init_Timer_delay(void);

//delay Functions using SysTick
void delay(int s, int d);
void delay_fraction(int d);
