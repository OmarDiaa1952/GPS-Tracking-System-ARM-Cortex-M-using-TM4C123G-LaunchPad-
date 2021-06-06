#include "TM4C123GH6PM.h"
#include "UART.h"
#include "GPIO.h"
#include "GPS.h"
#include "SYSTICK.h"
#include "DISPLAY.h"
#include "LCD.h"
#include "SERVER.h"

char latitude[15];
char longitude[15];
char lat_log[30];
//-------------------------------
volatile double fractions = 0; 
//-------------------------------

char data[100]; //General data holder

void LCD_message_R1(char *data);
void LCD_message_Dis(double dis);
void LCD_LatLogDis(char *lat,char *log,volatile double * dis);
void LCD_Log(char *data);
void led(int color);
int main(){
	char lat[15];
	char log[15];
	volatile double totalDistance = 0.0; //Total distance
	volatile double startEndCoords[4] = {0.0,0.0,0.0,0.0}; //Hold start and end point (latitudeStart,logStart,latEnd,logEnd)
	volatile int mode = 0;// 0-> Inactive, 1-> Initialization, 2-> Positioning, 3-> Active, 4-> Pre-reset
	volatile double distance = 0.0; //Hold distance every reading
	volatile double coords[4]= {0.0,0.0,0.0,0.0}; //Hold every 2 points (2 lats + 2 longs)
	volatile double startEnd[4]= {0.0,0.0,0.0,0.0}; //Hold start and end points
	volatile int  done = 0; //General purpose boolean
	
	UART5_Init(9600);
	UART7_Init(9600); //Change to 115200
	initPortF();
	initPortD();
	LCD_INIT();
	LCD_CMD(Function_set_8bit);
	
	LCD_message_R1("Inactive");
	//countingDistance(20.3,&fractions);
	delay(1,0);
	while(1){
		switch(mode){
			case 0: //Inactive mode
				if(!(GPIO_PORTF_DATA_R&0x01)){ mode = 1;LCD_message_R1("Initialization");}//If switch 2 pressed
			break;//Mode 0 break
				
			case 1: //Initialization mode
				
				if(!done){
					while(readGPS(lat,log)){}
					concat(lat_log,lat,log);
					led(12); //Turquoise
					done = 1;
				}else{
					UART7_ReadString(data,'\n'); 
					while(data[0] != '$'){UART7_ReadString(data,'\n');}
					if(data[1] == 'F'){done = 0;break;}
					if(data[1] == 'S'){
						mode = 2;
						LCD_message_R1("Positioning");
						splitAndStoreDouble(data,&startEnd[0],&startEnd[1],&startEnd[2],&startEnd[3]);
					}	
				}		
				break;//Mode 1 break
				
				case 2: //Positioning mode
					while(readGPS(lat,log)){}
					concat(lat_log,lat,log);
					led(10);
					if( (floatVal(lat) == startEnd[0]) && (floatVal(log) == startEnd[1]) ){
						coords[0] = floatVal(lat);
						coords[1] = floatVal(log);
						command('S',lat_log);
						delay(3,5);
						LCD_message_R1("Active");
						done = 1;
						mode = 3;
					}else{
						delay(2,0);
						mode = 2;
					}
			
			break;//Mode 2 break
					
			case 3: //Active mode
				while(readGPS(lat, log)){}
				concat(lat_log,lat,log);
				led(8);
				if(done){
					coords[2] = floatVal(lat);
					coords[3] = floatVal(log);
					done = 0;
				}else{
					coords[0] = floatVal(lat);
					coords[1] = floatVal(log);
					done = 1;
				}
				
				distance = calcDistance(coords[0],coords[1],coords[2],coords[3]);	
				totalDistance += distance;
				countingDistance(distance,&fractions);
				if( (floatVal(lat) == startEnd[2]) && (floatVal(log) == startEnd[3]) ){
					LCD_LatLogDis(lat,log,&distance);
					delay(2,1);
					command('E',lat_log);
					LCD_message_Dis(totalDistance);
					mode = 4;
				}else{
					command('C',lat_log);
					LCD_LatLogDis(lat,log,&distance);
					mode = 3;
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

	
void LCD_message_R1(char *data){
			LCD_CMD(clear_display);
			delay_milli(100);
			LCD_CMD(cursorOn);
			delay_milli(100);
			LCD_CMD(FirstRow);
			delay_milli(100);
			LCD_String(data);
			delay_milli(100);
}
void LCD_message_Dis(double dis){
			LCD_CMD(clear_display);
			delay_milli(15);
			LCD_CMD(FirstRow);
			delay_milli(15);
			LCD_CMD(cursorOn);
			delay_milli(15);
			LCD_String("Total Distance");
			delay_milli(15);
			LCD_CMD(SecondRow);
			delay_milli(15);
			LCD_CMD(cursorOn);
			delay_milli(15);
			LCD_String(toStringAyman(dis));
			LCD_String(" m");
}
void LCD_LatLogDis(char *lat,char *log,volatile double * dis){
			LCD_CMD(clear_display);
			delay_milli(15);
			LCD_CMD(FirstRow);
			delay_milli(15);
			LCD_CMD(cursorOn);
			delay_milli(15);
			LCD_String("Lat = ");
			LCD_String(lat);
			delay_milli(15);
			LCD_CMD(SecondRow);
			delay_milli(15);
			LCD_CMD(cursorOn);
			delay_milli(15);
			LCD_String("Log = ");
			LCD_String(log);
			delay(2,0);
			LCD_CMD(clear_display);
			delay_milli(10);
			LCD_CMD(FirstRow);
			delay_milli(10);
			LCD_String("Dis = ");
			LCD_String(toStringAyman(*dis));
			LCD_String(" m");
			delay(2,0);
}
