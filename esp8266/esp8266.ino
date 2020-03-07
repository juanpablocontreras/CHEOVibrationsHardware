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
    char temp;
    int index = 0;
    
    while(!Serial.available()){}//wait for data to start arriving
    do{
      if(Serial.available() && index < bufferArraySize){
        buffer_array[index++] = temp = Serial.read();
      }
    }while(temp != END_OF_RESPONSE && temp != END_OF_JSON);
    buffer_array[index] = NULL;//set last character to null
    
    //send data
    request->send(200,"text/plain",buffer_array);
  });


  // Start server
  server.begin();
  
}

void loop() {
  // put your main code here, to run repeatedly:
}
