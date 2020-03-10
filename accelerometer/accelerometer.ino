//Juan Pablo Contreras
//Accelerometer code


#include <MPU6050_tockn.h>
#include <Wire.h>

#define ACC_POLL_PERIOD 100 //poll accelerometer every ACC_POLL_PERIOD miliseconds

MPU6050 mpu6050(Wire);

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("\n");

  for(int i=0; i<10;i++){
   mpu6050.update();

    Serial.print("temp : ");Serial.println(mpu6050.getTemp());
    Serial.print("accX : ");Serial.print(mpu6050.getAccX());
    Serial.print("\taccY : ");Serial.print(mpu6050.getAccY());
    Serial.print("\taccZ : ");Serial.println(mpu6050.getAccZ());
    Serial.print("\n");
    delay(100);
   }
   delay(5000);//delay 5 seconds
}
