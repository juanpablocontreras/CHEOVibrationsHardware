//Juan Pablo Contreras
//esp8266 wifi code

#include "ESP8266WiFi.h"
#include "ESPAsyncWebServer.h"

//network credentials
const char* ssid = "transport-team";
const char* password = "cheo";


//Async web server on port 80
AsyncWebServer server(80);

//control variables
char END_OF_RESPONSE = '\n';
char END_OF_JSON = NULL;
int bufferArraySize = 200;
int pollingPeriod = 200;//time between polls to arduino in miliseconds


void setup() {
  //serial
  Serial.begin(115200);
  
  //creating access point
  WiFi.softAP(ssid, password);

  //server request handling
  server.on("/start",HTTP_GET, [](AsyncWebServerRequest *request){
    //start data collection for main functionality: to put data table of database for app refresh
    Serial.print("s");

    //get response from arduino 
    char buffer_array[bufferArraySize];
    char temp = '0';
    int index = 0;

    //wait for data to start arriving
    while(!Serial.available()){
      delay(pollingPeriod);
    }
    
    do{
      if(Serial.available() && index < bufferArraySize){
        buffer_array[index++] = temp = Serial.read();
      }
      delay(50); //added to prevent soft reset
    }while(temp != END_OF_RESPONSE && temp != END_OF_JSON);
    
    //send data
    request->send(200,"application/json",buffer_array);
  });

  server.on("/ack",HTTP_GET, [](AsyncWebServerRequest *request){
      //start data collection for main functionality: to put data table of database for app refresh
      Serial.print("a");

      //get response from arduino 
      char buffer_array[bufferArraySize];
      char temp = '0';
      int index = 0;
  
      //wait for data to start arriving
      while(!Serial.available()){
        delay(pollingPeriod);
      }
      
      do{
        if(Serial.available() && index < bufferArraySize){
          buffer_array[index++] = temp = Serial.read();
        }
        delay(50); //added to prevent soft reset
      }while(temp != END_OF_RESPONSE && temp != END_OF_JSON);
      
      //send data
      request->send(200,"application/json",buffer_array);
   });

   server.on("/end",HTTP_GET, [](AsyncWebServerRequest *request){
      //start data collection for main functionality: to put data table of database for app refresh
      Serial.print("e");

      //get response from arduino 
      char buffer_array[bufferArraySize];
      char temp = '0';
      int index = 0;
  
      //wait for data to start arriving
      while(!Serial.available()){
        delay(pollingPeriod);
      }
      
      do{
        if(Serial.available() && index < bufferArraySize){
          buffer_array[index++] = temp = Serial.read();
        }
        delay(50); //added to prevent soft reset
      }while(temp != END_OF_RESPONSE && temp != END_OF_JSON);
      
      //send data
      request->send(200,"application/json",buffer_array);
   });

  // Start server
  server.begin();
}

void loop() {
}
