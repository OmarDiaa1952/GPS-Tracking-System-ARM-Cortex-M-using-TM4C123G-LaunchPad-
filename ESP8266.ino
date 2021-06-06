#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const char* ssid = "GPS"; //Wifi ssid
const char* password = "megz1999"; //Wifi password
String serverName = "http://megz.125mb.com/commands.php"; //server

HTTPClient http;

void setup() {
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(9600); 
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN,LOW);
    delay(250);
    Serial.print(".");
    digitalWrite(LED_BUILTIN,HIGH);
    delay(250);
  }
  Serial.println();
  Serial.println();
  Serial.println();
  int i = 0;
  while(i < 3){i++;Serial.flush();delay(500);}
}
char data[50];
String res;
int i;
void loop() {
    //Check WiFi connection status
    while(WiFi.status()== WL_CONNECTED){
      serialReadString(data,'\n');   
      if(data[0] == '@'){
        switch(data[1]){
          case 'I': //Initialize
            res = sendCommand(data);
            if(res != "S"){Serial.println("@F,");}  // Will handle this later .... or maybe won't
            res = sendCommand("@G");
            while(res == "F"){
              delay(2500);
              res = sendCommand("@G");  
            }
            Serial.println();
            Serial.print(res);
          break;
          
          case 'S': //Start tracking
          case 'C': //Send coords
          case 'E': //End 
          Serial.println(data);
          res = sendCommand(data);
          Serial.println(res);
          break;

          case 'T': //Testing
          Serial.println(data);

          break;     
        }
      }//if starts with @

    

    delay(1000);
    }
    
}
void serialWriteChar(unsigned char ch){            //Write Char
  while(!Serial.availableForWrite()){}
  Serial.write(ch);
}

unsigned char serialReadChar(void){               //Read Char
  while(!Serial.available()){}
  return ((char)Serial.read());
}

void serialWriteString(char *str){                //Write String
  while(*str){
    serialWriteChar(*str);
    str++;
  }
}

void serialReadString(char *str, char stopCh){    //Read String
  char c = serialReadChar();
  while(c && c != stopCh){
    *str = c;
    str++;
    c = serialReadChar();
  }
  *str = 0x00;
}
String sendCommand(String command){
  String respond;
  WiFiClient wClient;
  http.begin(wClient, serverName);
  String httpRequestData = "command=" + command; //command=@hi 
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpResponseCode = http.POST(httpRequestData);
  if(httpResponseCode != 200){
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    Serial.println("@F,");
   }
  
  respond = http.getString();
  return respond;
}
