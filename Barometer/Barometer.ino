//Juan Pablo Contreras
//BMP180 Barometer

#include <Wire.h>
#include <Adafruit_BMP085.h>


Adafruit_BMP085 bmp;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  bmp.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Temperature = ");
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");

  Serial.print("Pressure = ");
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");

  Serial.println();
  delay(2000);
}
