//Juan Pablo Contreras
//works with Arduino Uno R3
#include <SoftwareSerial.h>
#include "ArduinoJson.h"
#include <MPU6050_tockn.h>
#include <Wire.h>

#define ARDUINO_SOFT_RX 4
#define ARDUINO_SOFT_TX 5

#define WIFI_POLL_PERIOD 200               //Poling period to see if there is new comand coming from wifi module
#define ACC_POLL_PERIOD 10                 //poll accelerometer every ACC_POLL_PERIOD miliseconds
#define ACC_NUM_POLLS 5                    //Number of times the accelerometer is polled to compute Vibration RMS


//control variables
bool is_Communication_On = false;

//Gloabl variables
SoftwareSerial esp8266(ARDUINO_SOFT_RX,ARDUINO_SOFT_TX); //RX, TX
char tabletCommand;
MPU6050 mpu6050(Wire);

//variables for main database
int time_of_collection = 0;
double temperature = 1;
int sound_level = 2;
double vibration_RMS;
double pressure = 4;
double humidity = 5;

//top level functions
void startDataAcquisition();
void ackDataAcquisition();
void endDataAcquisition();
void getRaw();
void getFiltered();

//middle level functions
void update_board();
void update_time();
void update_temperature_and_humidity();
void update_sound_level();
void update_vibraton_RMS();
void update_pressure();
void send_current_data();
void send_error();

//Bottom level funtions

void setup() {

  //serial and i2c
  Serial.begin(115200);//USB serial from arduino
  esp8266.begin(115200);//serial to esp8266 wifi module
  Wire.begin();//i2c bus begin

  //accelerometer
  mpu6050.begin();
  mpu6050.calcGyroOffsets();
}

void loop() {

  //wait for command from wifi module
  while(!esp8266.available()){
    delay(WIFI_POLL_PERIOD);
  }
  Serial.println("8266 available data!!");
  
  tabletCommand = esp8266.read();
  Serial.println(tabletCommand);

  switch(tabletCommand)
  {
      case 's':
        startDataAcquisition();
      break; 

      case 'a':
        ackDataAcquisition();
      break;

      case 'e':
        endDataAcquisition();
      break;

      case 'r':
        getRaw();
      break;

      case 'f':
        getFiltered();
      break;
      
      default:
      break;
   }
}


/*~~~~~ Interface functions: provide Interface function def and impl (Top Layer) ~~~~~~*/
void startDataAcquisition(){
    //start data collection for main functionality: to put data table of database for app refresh
    is_Communication_On = true;
    update_board();
    send_current_data();
    Serial.println("finished start");
}
void ackDataAcquisition(){
  if(is_Communication_On){
    update_board();
    send_current_data();
  }else{
    send_error();
  }
  Serial.println("finished ack");
}
void endDataAcquisition(){
    
    char buffer_array[100];
    const size_t capacity = JSON_OBJECT_SIZE(1);
    DynamicJsonDocument doc(capacity);
  
    doc["end"] = "end of data acquisition";

    serializeJson(doc, buffer_array, sizeof(buffer_array));
    esp8266.print(buffer_array);
    esp8266.print("\n");

    is_Communication_On = false;
    Serial.println("finished end");
}
void getRaw(){}
void getFiltered(){}

/*~~~~~ functions that interface with sensor board, including wifi module (middle layer) ~~~~~~~~~~~~~*/
void update_board(){
  update_time();
  update_vibraton_RMS();
  update_temperature_and_humidity();
  update_sound_level();
  update_pressure();
}
void update_time(){}
void update_temperature_and_humidity(){}
void update_sound_level(){}
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
void update_pressure(){}
void send_current_data(){
   //sends current data to esp8266 wifi module in JSON format using serial data communication
    Serial.println("sending current data...");
    
    char buffer_array[200];
    const size_t capacity = JSON_OBJECT_SIZE(6);
    DynamicJsonDocument doc(capacity);
  
    doc["time"] = time_of_collection;
    doc["temperature"] = temperature;
    doc["soundLevel"] = sound_level;
    doc["vibration"] = vibration_RMS;
    doc["pressure"] = pressure;
    doc["humidity"] = humidity;

    serializeJson(doc, buffer_array, sizeof(buffer_array));
    Serial.println(buffer_array);
    
    esp8266.print(buffer_array);
    esp8266.print("\n");
}
void send_error(){
    char buffer_array[150];
    const size_t capacity = JSON_OBJECT_SIZE(1);
    DynamicJsonDocument doc(capacity);
  
    doc["error"] = "400";

    serializeJson(doc, buffer_array, sizeof(buffer_array));
    esp8266.print(buffer_array);
    esp8266.print("\n");
}
