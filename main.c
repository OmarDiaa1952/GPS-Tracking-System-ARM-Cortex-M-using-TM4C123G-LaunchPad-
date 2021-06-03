#include "TM4C123GH6PM.h"
#include "UART.h"
#include "GPIO.h"
#include "GPS.h"


volatile int mode = 0;// 0-> Inactive, 1-> Initialization, 2-> Positioning, 3-> Active, 4-> Pre-reset

int main(){
	char lat[15]; //Latitude
	char log[15]; //Longitude
	volatile double dlat; //Double latitude
	volatile double dlog; //Double longitude
	volatile double distance = 0.0; //Hold distance every reading
	volatile double totalDistance = 0.0; //Total distance
	volatile double startEndCoords[4] = {0.0,0.0,0.0,0.0}; //Hold start and end point (latStart,logStart,latEnd,logEnd)
	volatile double coords[4]= {0.0,0.0,0.0,0.0}; //Hold every 2 points (2 lats + 2 longs)
	
	
	volatile char c; //General purpose char 
	volatile int  done = 0; //General purpose boolean 
	char data[50]; //General data holder
	volatile int i, j; //General purpose integers 
	
	UART5_Init(9600);
	UART7_Init(9600); //Change to 115200
	initPortF();
	
	while(1){
	
		switch(mode){
		case 0: //Inactive mode
			if(!(GPIO_PORTF_DATA_R&0x01)){ mode = 1;}//If switch 2 pressed			
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
							startEndCoords[0] = atof(lat);
							j = 0;
							
							while(data[i] != ';'){log[j] = data[i];i++;j++;}log[j] = 0x00;i++; //Get start log
							startEndCoords[1] = atof(log);
							j = 0;
							
							
						}else if (data[1] == 'E'){
							i = 3;j = 0;
							while(data[i] != ','){lat[j] = data[i];i++;j++;}lat[j] = 0x00;i++; //Get end lat
							startEndCoords[2] = atof(lat);
							j = 0;
							
							while(data[i] != ';'){log[j] = data[i];i++;j++;}log[j] = 0x00;i++; //Get end log
							startEndCoords[3] = atof(log);
							j = 0;
							
							for(i = 0;i < 4;i++){if(startEndCoords[i] == 0.0){break;}}
							mode = 2;j = 1;led(6);
						
						}
					}
					
					
					
			}
			break;//Mode 1 break
			
		
		
	}
	
	
	}
	

	
}


