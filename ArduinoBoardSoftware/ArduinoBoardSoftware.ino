//Board software using communication protocol 2.0
#include <Adafruit_BMP085.h>
#include <Wire.h>
#include <DHT.h>
#include <MPU6050_tockn.h>


#define SERIAL_POLLING_PERIOD   10
#define DHTPIN 2
#define DHTTYPE DHT11
#define SOUNDPIN A1

//Global variables and control variables
Adafruit_BMP085 bmp;
DHT dht(DHTPIN, DHTTYPE);
MPU6050 mpu6050(Wire);
float maxsound = 1024;
float maxsoundtemp = 1024;
double vdv = 0;
const float vdvExponent = 1.0/3.0;
long numMPUmeasurements = 0;
float x_calib = 0;
float y_calib = 0;
float z_calib = 0;

//functions
void send_time();
void send_temp();
void send_humidity();
void send_pressure();
void send_vib();
void send_sound();


void setup() {

  Serial.begin(9600);
  Wire.begin();
  
  bmp.begin();
  dht.begin();
  
  mpu6050.begin();
  calibrateMPU6050();
  
  
}

void loop() {

  while(!Serial.available()){
    //read values that need time series information (depend on more than one measurement)

    //Sound
    maxsoundtemp = analogRead(SOUNDPIN);
    if(maxsoundtemp < maxsound){
      maxsound = maxsoundtemp;
    }

    
    
    //accelerometer
    mpu6050.update();
    vdv += pow(pow(abs(mpu6050.getAccX()-x_calib),3) + pow(abs(mpu6050.getAccY()-y_calib),3) + pow(abs(mpu6050.getAccZ()-z_calib),3),vdvExponent);
    numMPUmeasurements++;
    
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
  Serial.print(dht.readHumidity());
  Serial.print("\n");  
}
void send_pressure(){
  Serial.print(bmp.readPressure());
  Serial.print("\n");  
}
void send_vib(){
  vdv = vdv/numMPUmeasurements;
  
  Serial.print(vdv);
  Serial.print("\n"); 
  
  numMPUmeasurements = 0;
  vdv = 0;   
}
void send_sound(){
  float sound = 1024 - maxsound;
  
  Serial.print(sound);
  Serial.print("\n");  
  maxsound = 1024;
  maxsoundtemp = 1024;
}
void calibrateMPU6050(){
   mpu6050.calcGyroOffsets(); //calculates gyro offsets and sets rates of gyro and acc
   mpu6050.update(); 
   x_calib = mpu6050.getAccX();
   y_calib = mpu6050.getAccY();
   z_calib = mpu6050.getAccZ();
}
