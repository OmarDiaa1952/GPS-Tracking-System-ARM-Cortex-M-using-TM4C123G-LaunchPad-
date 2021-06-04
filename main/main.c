#include "TM4C123GH6PM.h"
#include "UART.h"
#include "GPIO.h"
#include "GPS.h"
#include "DELAY.h"

volatile int mode = 0;// 0-> Inactive, 1-> Initialization, 2-> Positioning, 3-> Active, 4-> Pre-reset

void led(int color);
int main(){
	char lat[15]; //Latitude
	char log[15]; //Longitude
	 double dlat; //Double latitude
	 double dlog; //Double longitude
	volatile double distance = 0.0; //Hold distance every reading
	volatile double totalDistance = 0.0; //Total distance
	volatile double startEndCoords[4] = {0.0,0.0,0.0,0.0}; //Hold start and end point (latStart,logStart,latEnd,logEnd)
	//volatile double coords[4]= {0.0,0.0,0.0,0.0}; //Hold every 2 points (2 lats + 2 longs)
	double lat1 = 0.0;
	double log1 = 0.0;
	double lat2 = 0.0;
	double log2 = 0.0;
	
	volatile char c; //General purpose char 
	volatile int  done = 0; //General purpose boolean 
	char data[50]; //General data holder
	volatile int i, j, k; //General purpose integers 
	
	UART5_Init(9600);
	UART7_Init(9600); //Change to 115200
	initPortF();
	
	while(1){
	
		switch(mode){
		case 0: //Inactive mode
			if(!(GPIO_PORTF_DATA_R&0x01)){ mode = 1;}//If switch 2 pressed
			
			
			//distance = calcDistance(30.10527,31.31637,30.10522,31.31657);	
			
			break;//Mode 0 break
			
			
		case 1: //Initialization mode
			if(!done){
				readGPS(lat, log); //Get current position to send it to server
				if(lat[0]){
					if(lat[0] < '0' || lat[0] > '9'){break;}
					if(log[0] < '0' || lat[0] > '9'){break;}
					
					i = 0;
					j = 0;
					while(lat[i]){data[i] = lat[i];i++;} //Concat lat and log in data
					data[i] = ',';i++;j=0;
					while(log[j]){data[i] = log[j];j++;i++;}
					data[i] = 0x00;
					led(12); //Turquoise
					UART7_WriteString("@I,");
					UART7_WriteString(data);
					UART7_WriteString("\n");
					done = 1;
				}
			}else{
					UART7_ReadString(data,'\n'); 
					//Expected message => $S,latitudeStart,longitudeStart; (or) $E,latitudeEnd,longitudeEnd;
					if(data[0] == '$'){
						if(data[1] == 'S'){ //Start point
							
							i = 3;j = 0;
							while(data[i] != ','){lat[j] = data[i];i++;j++;}lat[j] = 0x00;i++; //Get start lat
							startEndCoords[0] = floatVal(lat);
							j = 0;
							
							while(data[i] != ';'){log[j] = data[i];i++;j++;}log[j] = 0x00;i++; //Get start log
							startEndCoords[1] = floatVal(log);
							j = 0;
							
							
						}else if (data[1] == 'E'){
							i = 3;j = 0;
							while(data[i] != ','){lat[j] = data[i];i++;j++;}lat[j] = 0x00;i++; //Get end lat
							startEndCoords[2] = floatVal(lat);
							j = 0;
							
							while(data[i] != ';'){log[j] = data[i];i++;j++;}log[j] = 0x00;i++; //Get end log
							startEndCoords[3] = floatVal(log);
							j = 0;
							
							for(i = 0;i < 4;i++){if(startEndCoords[i] == 0.0){break;}}
							mode = 2;j = 1;led(6);
						
						}
					}
					
					
					
			}
			break;//Mode 1 break
			
		case 2: //Positioning mode	
			readGPS(lat, log);
			if(lat[0]){
					if(lat[0] < '0' || lat[0] > '9'){break;}
					if(log[0] < '0' || lat[0] > '9'){break;}
					dlat = floatVal(lat);
					dlog = floatVal(log);
					
					i = 0;
					j = 0;
					while(lat[i]){data[i] = lat[i];i++;} 
					data[i] = ',';i++;j=0;
					while(log[j]){data[i] = log[j];j++;i++;}
					data[i] = 0x00;
					
		
					if(dlat == startEndCoords[0] && dlog == startEndCoords[1]){
						lat1 = dlat;
						log1 = dlog;
						
						mode = 3;k=1;
						UART7_WriteString("@S,");
						UART7_WriteString(data);
						UART7_WriteString("\n");
					
					
					}
				
				}else{
					delay(1,0);
				}
			break;//Mode 2 break
		
		case 3: //Active mode
			readGPS(lat, log); 		
			if(lat[0]){
					led(8);
					if(lat[0] < '0' || lat[0] > '9'){break;}
					if(log[0] < '0' || lat[0] > '9'){break;}
					dlat = floatVal(lat);
					dlog = floatVal(log);
			
					if(k){
						lat2 = dlat;
						log2 = dlog;
						k = 0;
					}else{
						lat1 = dlat;
						log1 = dlog;
						k = 1;
					}
					
					distance = calcDistance(lat1,log1,lat2,log2);	
					totalDistance += distance;
				
					//Update seven segment
					
					i = 0;
					j = 0;
					while(lat[i]){data[i] = lat[i];i++;} 
					data[i] = ',';i++;j=0;
					while(log[j]){data[i] = log[j];j++;i++;}
					data[i] = 0x00;
					
					
					if(dlat == startEndCoords[2] && dlog == startEndCoords[3]){
						mode = 4;
						UART7_WriteString("@E,");
						UART7_WriteString(data);
						UART7_WriteString("\n");
					}else{
						UART7_WriteString("@C,");
						UART7_WriteString(data);
						UART7_WriteString("\n");
						mode = 3;
					}
					
					
				}
		
		break;	//Mode 3 break
				
				
		case 4: //Pre-reset mode
			led(6);
			delay(1,0);
			led(0);
			delay(1,0);
		break; //Mode 4 break
		
		
	}
	
	
	}
	

	
}


