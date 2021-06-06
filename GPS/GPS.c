#include "TM4C123GH6PM.h"
#include "UART.h"
#include "GPIO.h"
#include "GPS.h"
#include "SYSTICK.h"
#include "SERVER.h"
#include <math.h>

int readGPS(char * lat, char * log){ // 1 ==> error , 0 ==> good
	//$GPRMC,200751.00,A,4047.32510,N,02929.63031,E,9.879,105.80,301117,,,A*6C
	char data[100];
	int i = 8;
	*lat = 0x00;
	UART5_ReadString(data,'\n');
	if(data[0] == '$'){
		if(data[3] == 'R'){
			if( (data[7] >= '0') && (data[7] <= '9') ){
				while(data[i] != ','){i++;}i++;
				if( (data[i] == 'A') || (data[i] == 'V') ){
					i += 2;
					while(data[i] != ','){*lat = data[i];lat++;i++;}
					*lat = 0x00;i += 3;
						while(data[i] != ','){*log = data[i];log++;i++;}
						*lat = 0x00;
						return 0;
				}else {led(2);return 1;}//Corrupted Data
			}else{led(4);delay(1,0);led(0);delay(1,0);return 1;}//GPS not ready
		}else {return 1;}
	}else{led(2);return 1;}//Corrupted Data
	
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
    while(*c){if(*c == '.'){p = i;c++;continue;}holder[i] = *c;i++;c++;}
    holder[i] = '\0';
    for(j = 0;j < i;j++){result += (int)(holder[i-j-1] - '0') * pow(10,j);}
    p = i - p;
    result /= pow(10,p);
    return result;
}

void readGPSDouble(volatile double * dlat,volatile double * dlog){
	char lat[15];
	char log[15];
	readGPS(lat,log);
	while(!lat[0]){readGPS(lat,log);}
	*dlat = floatVal(lat);
	*dlog = floatVal(log);
}

void splitAndStoreDouble(char * data, volatile double * startLat, volatile double * startLog, volatile double * endLat, volatile double * endLog){
	int i = 3;int j = 0;
	char string[15];
	
	while(data[i] != ','){string[j] = data[i];j++;i++;}string[j] = 0x00;
	*startLat = floatVal(string);j = 0;i++;
	
	while(data[i] != ','){string[j] = data[i];j++;i++;}string[j] = 0x00;
	*startLog = floatVal(string);j = 0;i++;
	
	while(data[i] != ','){string[j] = data[i];j++;i++;}string[j] = 0x00;
	*endLat = floatVal(string);j = 0;i++;
	
	while(data[i] != ';'){string[j] = data[i];j++;i++;}string[j] = 0x00;
	*endLog = floatVal(string);

}
	

void concat(char * holder, char * lat, char *log){
	while(*lat){*holder = *lat;holder++;lat++;}
	*holder = ',';holder++;
	while(*log){*holder = *log;holder++;log++;}
	*holder = 0x00;
}
