//Board software using communication protocol 2.0
#include <SoftwareSerial.h>
#include <Adafruit_BMP085.h>
#include <Wire.h>




#define SERIAL_POLLING_PERIOD   20

//Global variables and control variables
Adafruit_BMP085 bmp;

//functions
void send_time();
void send_temp();
void send_humidity();
void send_pressure();
void send_vib();
void send_sound();


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  bmp.begin();
}

void loop() {

  while(!Serial.available()){
      delay(SERIAL_POLLING_PERIOD);
  }

  char cmd = Serial.read();

  switch(cmd)
  {
      case 'T':
        send_time();
        break;
        
      case 't':
        send_temp();
        break;

      case 'h':
        send_humidity();
        break;

      case 'p':
        send_pressure();
        break;

      case 'v':
        send_vib();
        break;

      case 's':
        send_sound();
        break;
      
  }
}

void send_time(){
  Serial.print(millis());
  Serial.print("\n");
}

void send_temp(){
  
  Serial.print(bmp.readTemperature());
  Serial.print("\n");
}
void send_humidity(){
  float humidity = 60.5;
  
  Serial.print(humidity);
  Serial.print("\n");  
}
void send_pressure(){
  float pressure = 1023.7;
  
  Serial.print(pressure);
  Serial.print("\n");  
}
void send_vib(){
  float vib = 0.45;
  
  Serial.print(vib);
  Serial.print("\n");    
}
void send_sound(){
  float sound = 875;
  
  Serial.print(sound);
  Serial.print("\n");  
}
