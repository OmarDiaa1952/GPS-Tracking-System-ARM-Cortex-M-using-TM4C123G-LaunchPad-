#include "TM4C123GH6PM.h"
#include "UART.h"
#include "GPIO.h"
#include "GPS.h"
#include "SYSTICK.h"
#include "DISPLAY.h"
#include "LCD.h"
#include "SERVER.h"
#include "math.h"

char latitude[15];
char longitude[15];
char lat_log[30];
char lat_log_dis[50];
int i = 0;
//-------------------------------
volatile double fractions = 0; 
//-------------------------------
volatile double totalDistance = 0.0; //Total distance
char data[100]; //General data holder

void LCD_message_R1(char *data);
void LCD_message_Dis(double dis);
void LCD_LatLogDis(double lat,double log,volatile double dis);
void LCD_Log(char *data);
void led(int color);
int main(){
	char lat[15];
	char log[15];
	
	volatile int mode = 0;// 0-> Inactive, 1-> Initialization, 2-> Positioning, 3-> Active, 4-> Pre-reset
	volatile double distance = 0.0; //Hold distance every reading
	volatile double coords[4]= {0.0,0.0,0.0,0.0}; //Hold every 2 points (2 lats + 2 longs)
	volatile double startEnd[4]= {0.0,0.0,0.0,0.0}; //Hold start and end points
	volatile int  done = 0; //General purpose boolean
	
	UART5_Init(9600);
	UART7_Init(9600); //Change to 115200
	UART0_Init(9600);
	initPortF();
	initPortD();
	LCD_INIT();
	LCD_CMD(Function_set_8bit);
	
	LCD_message_R1("Inactive");
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
					command('I',lat_log);
					done = 1;
				}else{
					UART0_ReadString(data,'\n'); 
					while(data[0] != '$'){UART0_ReadString(data,'\n');}
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
					led(10);
						coords[0] = convertCoords(floatVal(lat));
						coords[1] = convertCoords(floatVal(log));
					if( (fabs(coords[0] - startEnd[0]) < 0.0002) && (fabs(coords[1] - startEnd[1]) < 0.0002) ){
						command('S',lat_log);
						delay(3,5);
						LCD_message_R1("Active");
						done = 1;
						mode = 3;
					}else{		
						delay(1,0);
						mode = 2;
					}
			
			break;//Mode 2 break
					
			case 3: //Active mode
				while(readGPS(lat, log)){}
				concat(lat_log,lat,log);				
				led(8);
					
					if(coords[2] == 0.0){
						coords[2] = convertCoords(floatVal(lat));
						coords[3] = convertCoords(floatVal(log));
					}else{
						coords[0] = coords[2];
						coords[1] = coords[3];
						coords[2] = convertCoords(floatVal(lat));
						coords[3] = convertCoords(floatVal(log));
					}
				
				UART0_WriteString("Distance between :");
					UART0_WriteString(toString(coords[0],7));
				UART0_WriteString(", ");
				UART0_WriteString(toString(coords[1],7));
				UART0_WriteString(" AND ");
				UART0_WriteString(toString(coords[2],7));
				UART0_WriteString(", ");
				UART0_WriteString(toString(coords[3],7));
					
				
				distance = calcDistance(coords[0],coords[1],coords[2],coords[3]);
				distance = distance < 1.0?0.0:distance;
				totalDistance = totalDistance + distance;
					
				UART0_WriteString(" is ");
					UART0_WriteString(toString(distance,4));
				UART0_WriteString(" AND total distance = ");
				UART0_WriteString(toString(totalDistance,4));
					UART0_WriteString("\n");
				
				
				
				
				//countingDistance(distance,&fractions);
				
				
				LCD_LatLogDis(coords[2],coords[3],totalDistance);				
				if( (fabs(coords[2] - startEnd[2]) < 0.0002) && (fabs(coords[3] - startEnd[3]) < 0.0002) ){
					delay(2,1);
					UART0_WriteString("@E,");
					UART0_WriteString(lat_log);
					UART0_WriteString(",");
					UART0_WriteString(toString(totalDistance,3));
					UART0_WriteString("\n");
				
					LCD_message_Dis(totalDistance);
					mode = 4;
				}else{
					UART0_WriteString("@C,");
					UART0_WriteString(lat_log);
					UART0_WriteString(",");
					UART0_WriteString(toString(totalDistance,3));
					UART0_WriteString("\n");					
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
			LCD_String(toString(dis,2));
			LCD_String(" m");
}
void LCD_LatLogDis(double lat,double log,volatile double dis){
			LCD_CMD(clear_display);
			delay_milli(15);
			LCD_CMD(FirstRow);
			delay_milli(15);
			LCD_CMD(cursorOn);
			delay_milli(15);
			LCD_String("Lat = ");
			LCD_String(toString(lat,5));
			delay_milli(15);
			LCD_CMD(SecondRow);
			delay_milli(15);
			LCD_CMD(cursorOn);
			delay_milli(15);
			LCD_String("Log = ");
			LCD_String(toString(log,5));
			delay(2,0);
			LCD_CMD(clear_display);
			delay_milli(10);
			LCD_CMD(FirstRow);
			delay_milli(10);
			LCD_String("Dis = ");
			LCD_String(toString(dis,2));
			LCD_String(" m");
			delay(2,0);
}
