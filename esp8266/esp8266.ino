//Juan Pablo Contreras
//esp8266 wifi code

#include "ESP8266WiFi.h"
#include "ESPAsyncWebServer.h"
#include "ArduinoJson.h"

#define SERIAL_POLL_PERIOD 20
#define LOOP_DELAY 20

//network credentials
const char* ssid = "cheo-transport-team";
const char* password = "che0_best_team";

//Global variables
char END_OF_RESPONSE = '\n';
AsyncWebServer server(80);      //Async web server on port 80

//function declarion
String getTime();
String getTemperature();
String getHumidity();
String getPressure();
String getVibs();
String getFromSerial();

void setup() {

  //built in led
  pinMode(LED_BUILTIN,OUTPUT);
  
  //serial
  Serial.begin(9600);
  
  //creating access point
  WiFi.softAP(ssid, password);

  //server request handling
  server.on("/app",HTTP_GET, [](AsyncWebServerRequest *request){

    //turn on LED
    digitalWrite(LED_BUILTIN,HIGH);
    
    //start data collection for main functionality: to put data table of database for app refresh
    char jsonBuffer[200];
    const size_t capacity = JSON_OBJECT_SIZE(6);
    DynamicJsonDocument doc(capacity);
    
    doc["time"] = getTime();
    doc["temperature"] = getTemperature();
    doc["soundLevel"] = getSound();
    doc["vibration"] = getVibs();
    doc["pressure"] = getPressure();
    doc["humidity"] = getHumidity();
    
    //send data
    request->send(200,"application/json",jsonBuffer);

    digitalWrite(LED_BUILTIN,LOW);
  });

  // Start server
  server.begin();
}

void loop() {
  delay(LOOP_DELAY);
}

String getTime(){
  Serial.print("T");
  return getFromSerial();  
}
String getTemperature(){
  Serial.print("t");
  return getFromSerial();
}
String getHumidity(){
  Serial.print("h");
  return getFromSerial();  
}
String getPressure(){
  Serial.print("p");
  return getFromSerial();  
}
String getVibs(){
  Serial.print("v");
  return getFromSerial();    
}
String getSound(){
  Serial.print("s");
  return getFromSerial();    
}
String getFromSerial(){
  String response = "";
  char temp;

  do
  {
      //wait for serial data
      while(!Serial.available()){
        delay(SERIAL_POLL_PERIOD); 
      }

      //serial data available
      temp = Serial.read();
      if(temp != END_OF_RESPONSE){
         response += String(temp);
      }
  }while(temp != END_OF_RESPONSE);
  
  return response;
}
