//Juan Pablo Contreras
//BMP180 Barometer

#include <Wire.h>
#include <Adafruit_BMP085.h>


Adafruit_BMP085 bmp;
double pressure;
void update_pressure();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  bmp.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

  update_pressure();
  Serial.println(pressure);
  delay(2000);
}


void update_pressure(){
  pressure = bmp.readPressure();
}
