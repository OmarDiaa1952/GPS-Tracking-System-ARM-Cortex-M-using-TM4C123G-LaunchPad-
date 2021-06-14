#include "SYSTICK.h"
#include "TM4C123GH6PM.h"

//functions for counting
void pulse(void);
void countingDistance(double L,volatile double *fraction);
void resetCounters(void);

