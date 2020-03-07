//Juan Pablo Contreras
//Wifi protocol to interface with esp8266

#include <SoftwareSerial.h>

//esp8266 serial variables
SoftwareSerial esp8266(2,3); //RX, TX
char tabletCommand;


//function declaration
void startDataAcquisition();
void ackDataAcquisition();
void endDataAcquisition();
void getRaw();

void setup() {

  //serial
  esp8266.begin(115200);//serial to esp8266 wifi module
  
}

void loop() {

  //wait for command from wifi module
  while(!esp8266.available()){}
  tabletCommand = esp8266.read();

  switch(tabletCommand)
  {
      case 's':
        startDataAcquisition();
      break; 

      case 'a':
        ackDataAcquisition();
      break;

      case 'e':
        endDataAcquisition();
      break;

      case 'r':
        getRaw();
      break;

      default:
      break;
   }
}


void startDataAcquisition(){
  esp8266.print("started data acquisition");
  esp8266.print('\n');
}

void ackDataAcquisition(){
   esp8266.print("ack data acquisition");
   esp8266.print('\n');
}


void endDataAcquisition(){
   esp8266.print("ended data acquisition");
   esp8266.print('\n');
}


void getRaw(){

}
