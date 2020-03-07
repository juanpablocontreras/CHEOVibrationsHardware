//Juan Pablo Contreras
//esp8266 wifi code

#include "ESP8266WiFi.h"
#include "ESPAsyncWebServer.h"

//network credentials
const char* ssid = "transport-team";
const char* password = "cheo";


//Async web server on port 80
AsyncWebServer server(80);


void setup() {
  //serial
  Serial.begin(115200);
  
  //creating access point
  WiFi.softAP(ssid, password);
  
  //Get IP address and display it over serial
  IPAddress IP = WiFi.softAPIP();
  Serial.println();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  //server request handling
  server.on("/start",HTTP_GET, [](AsyncWebServerRequest *request){
    //start data collection for main functionality: to put data table of database for app refresh
    Serial.print("s");
    Serial.print("\n");

    //wait for response from arduino 
    char termination = 'f';
    int index = 0;
    char buffer_array[200];
    
    while(!Serial.available()){}//wait for data to start arriving
    while(termination != NULL){
      //read data untill null character is encountered
      if(Serial.available()){
        buffer_array[index++] = termination = Serial.read();
      }
    }
    buffer_array[index] = NULL;//set last character to null
    
    //send data as json
    request->send(200,"application/json",buffer_array);
  });


  // Start server
  server.begin();
  
}

void loop() {
  // put your main code here, to run repeatedly:
}
