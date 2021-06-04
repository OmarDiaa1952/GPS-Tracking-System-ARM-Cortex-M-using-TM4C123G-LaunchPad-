#include "DisplayingDistance.h"

void pulse(void)  //make pulse for decoder to start counting
{
	//put PD0 pin as 0 then delay 0.1 sec then 1 and delay 0.1 sec and return 0 for 0.1sec this is the pulse function and will genereate pulse
	GPIO_PORTD_DATA_R &= ~(0x00000001);
	delay(0, 1);  //delay 0.1 secound
	GPIO_PORTD_DATA_R |=  (0x00000001);
	delay(0, 1);  //delay 0.1 secound
	GPIO_PORTD_DATA_R &= ~(0x00000001);
	delay(0, 1);  //delay 0.1 secound >> so now drwawed the pulse of PD0 which will start the counter to increment in 0.3secounds
	return;
}

void resetCounters(void)   //reset all counters of the 3 decoders so seven segment displays 000
{
	GPIO_PORTD_DATA_R |= 0x0000000E; // PE1,2,3 set to 1 >> 01110 1 at reset button so will reset decoder
	return;
}

void countingDistance(double L)    // L is distance between 2 points so it could be 2.23 so it's intger and fraction part and displayed on 7 segment
{
	int d = (int) L;									//intger part will be saved in d then displayed in line 97>>101
	fractions = fractions +  (L - (int)L);  //fraction will be summed till it reach 1 then make pulse where its ranges 0.02>>>>1.98
	while(d)
	{		
		pulse(); //generate pulse the increase 7 segment number
		d--;
	}
	if (fractions >=1)      //range from 0.02 to 1.98 is still need to be summed once more so won't go here in the loop but from 1>>>1.98 will be as 1 pulse
	{
		pulse();
		fractions--;  //from 1>>1.98 minused by 1 so 0>>0.98 and be added again
	}
	return;
}
/*void countingDistance2(double * td, int *dd) // td = 5.99 >> dd = 2 >>>>> 7segment 3aleha 5 >> 8
{
	while ((int)*td > *dd)
	{
		pulse();
		*dd = *dd + 1;
	}
}*/
