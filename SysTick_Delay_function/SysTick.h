#include "tm4c123gh6pm.h" // that has all timer files

// SysTick intialization
void init_Timer_delay(void);

//delay Functions using SysTick
void Delay_Of_Fractions(int frac);
void delay(int s, int d);

//delay MicroSeconds
void hundredMicroSecounds(void);
