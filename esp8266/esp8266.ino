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
  WiFi.softAP("transport-team", "cheo");
  
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
    //char buffer_array[200] = "started";
    
    request->send(200,"text/plain","started");
  });


  server.on("/ack",HTTP_GET, [](AsyncWebServerRequest *request){
    //start data collection for main functionality: to put data table of database for app refresh
    Serial.print("a");
    Serial.print("\n");

    //wait for response from arduino 
    //char buffer_array[200] = "started";
    
    request->send(200,"text/plain","ack");
  });

    server.on("/end",HTTP_GET, [](AsyncWebServerRequest *request){
    //start data collection for main functionality: to put data table of database for app refresh
    Serial.print("e");
    Serial.print("\n");

    //wait for response from arduino 
    
    
    request->send(200,"text/plain","end");
  });


  // Start server
  server.begin();
  
}

void loop() {
  // put your main code here, to run repeatedly:
}
