//Juan Pablo Contreras
//Accelerometer code


#include <MPU6050_tockn.h>
#include <Wire.h>

#define ACC_POLL_PERIOD 10            //poll accelerometer every ACC_POLL_PERIOD miliseconds
#define ACC_NUM_POLLS 10              //Number of times the accelerometer is polled to compute Vibration RMS

//variables for main database
double vibration_RMS;

//functions
void update_vibraton_RMS();


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

  update_vibraton_RMS();
  Serial.println(vibration_RMS);
  delay(5000);
}


void update_vibraton_RMS(){
  float acc_x2sum = 0;  //sum of squares of x acceleration readings
  float acc_y2sum = 0;  //sum of squares of y acceleration readings
  float acc_z2sum = 0;  //sum of squares of z acceleration readings
  float curr_reading;

  
  for(int i = 0; i< ACC_NUM_POLLS; i++){
     mpu6050.update();

     curr_reading = mpu6050.getAccX();
     acc_x2sum +=  curr_reading * curr_reading;

     curr_reading = mpu6050.getAccY();
     acc_y2sum += curr_reading * curr_reading;

     curr_reading = mpu6050.getAccZ() - 1.0;  //substract 1.0 beacause of gravity
     acc_z2sum += curr_reading * curr_reading;

     delay(ACC_POLL_PERIOD);
  }
  
  //compute RMS in each direction
  acc_x2sum = acc_x2sum / (float)ACC_NUM_POLLS;
  acc_y2sum = acc_y2sum / (float)ACC_NUM_POLLS;
  acc_z2sum = acc_z2sum / (float)ACC_NUM_POLLS;

  //compute vibrations RMS
  vibration_RMS = sqrt((acc_x2sum + acc_y2sum + acc_z2sum)/3.0);
}
