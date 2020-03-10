//Juan Pablo Contreras
//works with Arduino Uno R3
#include <SoftwareSerial.h>
#include "ArduinoJson.h"

#define ARDUINO_SOFT_RX 2
#define ARDUINO_SOFT_TX 3

//esp8266 serial variables
SoftwareSerial esp8266(ARDUINO_SOFT_RX,ARDUINO_SOFT_TX); //RX, TX
char tabletCommand;

//variables for main database
int time_of_collection = 0;
double temperature = 1;
int sound_level = 2;
double vibration_RMS = 3;
double pressure = 4;
double humidity = 5;

//global control variables
bool is_Communication_On = false;
int pollingPeriod = 200;  //time between polls to arduino in miliseconds

//function declaration
void startDataAcquisition();
void ackDataAcquisition();
void endDataAcquisition();
void getRaw();
void getFiltered();
void send_current_data();
void send_error();

void setup() {

  //serial
  esp8266.begin(115200);//serial to esp8266 wifi module
  Serial.begin(115200);//USB serial
}

void loop() {

  //wait for command from wifi module
  while(!esp8266.available()){
    delay(pollingPeriod);
  }
  Serial.println("8266 available data!!");
  
  tabletCommand = esp8266.read();
  Serial.println(tabletCommand);

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

      case 'f':
        getFiltered();
      break;
      
      default:
      break;
   }
}


void startDataAcquisition(){
    //start data collection for main functionality: to put data table of database for app refresh
    is_Communication_On = true;
    send_current_data();
    Serial.println("finished start");
}

void ackDataAcquisition(){
  if(is_Communication_On){
    send_current_data();
  }else{
    send_error();
  }
  Serial.println("finished ack");
}


void endDataAcquisition(){
    
    char buffer_array[100];
    const size_t capacity = JSON_OBJECT_SIZE(1);
    DynamicJsonDocument doc(capacity);
  
    doc["end"] = "end of data acquisition";

    serializeJson(doc, buffer_array, sizeof(buffer_array));
    esp8266.print(buffer_array);
    esp8266.print("\n");

    is_Communication_On = false;
    Serial.println("finished end");
}


void getRaw(){

}

void getFiltered(){}

void send_current_data(){
    Serial.println("sending current data...");
    
    char buffer_array[200];
    const size_t capacity = JSON_OBJECT_SIZE(6);
    DynamicJsonDocument doc(capacity);
  
    doc["time"] = time_of_collection;
    doc["temperature"] = temperature;
    doc["soundLevel"] = sound_level;
    doc["vibration"] = vibration_RMS;
    doc["pressure"] = pressure;
    doc["humidity"] = humidity;

    serializeJson(doc, buffer_array, sizeof(buffer_array));
    Serial.println(buffer_array);
    
    esp8266.print(buffer_array);
    esp8266.print("\n");
}

void send_error(){
    char buffer_array[150];
    const size_t capacity = JSON_OBJECT_SIZE(1);
    DynamicJsonDocument doc(capacity);
  
    doc["error"] = "400";

    serializeJson(doc, buffer_array, sizeof(buffer_array));
    esp8266.print(buffer_array);
    esp8266.print("\n");
}
