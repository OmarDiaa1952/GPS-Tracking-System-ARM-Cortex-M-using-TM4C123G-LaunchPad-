#include "TM4C123GH6PM.h"
#include "UART.h"
#include "GPIO.h"
#include "GPS.h"
#include <math.h>


void readGPS(char * lat, char * log){
	//$GPRMC,200751.00,A,4047.32510,N,02929.63031,E,9.879,105.80,301117,,,A*6C
	char data[100];
	int i = 8;
	*lat = 0x00;
	UART5_ReadString(data,'\n');
	if(data[3] == 'R'){
		if(data[7] != ','){ //Check if GPS ready
			while(data[i] != ','){i++;} //We don't need time part (200751.00)
			if(data[i+1] != 'A' && data[i+1] != 'V'){
			led(2);return;}
			i += 3; //Jump to 4047.32510 skipping (,A,)
			while(data[i] != ','){
				*lat = data[i];
				lat++;
				i++;
			}
			*lat = 0x00; //Terminate string
			i += 3; //Jump to 02929.63031 skipping (,N,)
			while(data[i] != ','){
				*log = data[i];
				log++;
				i++;
			}
			*log = 0x00; //Terminate string
		}else{
			led(4);//Blue led
			*lat = 0x00; //Not ready
			return;
		}
	}
}

double calcDistance(volatile double latitude_1, volatile double logitude_1, volatile double latitude_2, volatile double logitude_2){
  volatile double phi1, phi2, delta_phi, delta_lmbda, a, c, d;
	phi1 = DEG_TO_RAD(latitude_1);
	phi2 = DEG_TO_RAD(latitude_2);
  delta_phi = phi2-phi1;
  delta_lmbda = logitude_2-logitude_1;
  delta_lmbda = DEG_TO_RAD(delta_lmbda);

   a = (sin(delta_phi/2) * sin(delta_phi/2)) + 
       (cos(phi1) * cos(phi2)) *
       (sin(delta_lmbda/2) * sin(delta_lmbda/2)) ;
   c = 2 * atan2(sqrt(a),sqrt(1-a));
   d = EARTH_RADIUS * c;
	return d;
}

double floatVal(char *c){
    char holder[10]; //215.34
    double result = 0;
    int i = 0;
    int p = 0;
		int j;
    while(*c){
        if(*c == '.'){p = i;c++;continue;}
        holder[i] = *c;
        i++;
        c++;
    }
    holder[i] = '\0';
    for(j = 0;j < i;j++){
         result += (int)(holder[i-j-1] - '0') * pow(10,j);
    }
    p = i - p;
    result /= pow(10,p);
    return result;
}
