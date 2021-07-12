// IOWLabs - UDD
// VMM proyect -  Pressure sensor lib test.


// For Arduino ESP32

// Connected modules:
//  - Pressure sensor (I2C)

// Arduino serial communication
//  Baud rate:  115200 BPS
//  protocol: based on json format
//  Structure:
//    {'cmd':' ','param1':' ','param2':' ','param3':' ',...}
//  Commands:
//  Base:
//    help
//    echo  [0/1]
//    reset
//    version
//    uptime
//    ip
//    mac
//  Application specific:
//    get_temperature  

// Memory usage:
//    Sketch uses 37838 bytes (14%) of program storage space. Maximum is 253952 bytes.
//    Global variables use 3947 bytes (48%) of dynamic memory, leaving 4245 bytes for local variables. Maximum is 8192 bytes.


// Version (mayor, minor, patch, build)
#define VERSION    "VMM - v.0.0.0.b.0"

//-----------
// Defines
//-----------
// Enable functionalities
// ifdef:  https://forum.arduino.cc/index.php?topic=46900.0
// Comment correspondent define to disable functionality
//#define ENABLE_PUMP         // Pump generic control
//#define ENABLE_PRESSURE     // Presurre sensor
//#define ENABLE_O2           // O2 gas sensor
//#define ENABLE_TEMPERATURE  // Temperature sensor

//-----------
// Libraries
//-----------
#include <Arduino.h>
#include <Wire.h>        // Necesary for the pressure sensor
//#include <ArduinoJson.h> // Lib to code and decode  Json format on arduino


#define DEBUG


#define PRESSURE_MODE 0 // read pressure and temperature
#define PARCIAL_TEMP_MODE 1 // read pressure and temperature
#define ALL_FRAME_MODE 2 // read pressure and temperature
#define SENSOR_ADDR  0x28 // from 
#define FSS_H20 60 // from datasheet page 1
#define OS_DIG  8192 // from datasheet page 3
#define PS_INT 4
#define BUFF_SIZE 10

uint8_t mode = ALL_FRAME_MODE;
byte data[4];

int n = 0;

struct pressureSensorValues{
  float temp;
  float press;
  float temp_avg;
  float press_avg;
  uint8_t error;
};

int temp_raw = 0;
int press_raw= 0;
int idx_sample=0;
int idx=0;

float temp_buff[BUFF_SIZE];
float press_buff[BUFF_SIZE];


pressureSensorValues psv =  {0,0,0,0,0};

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  Serial.println("starting");
 
}


void loop()
{
  delay(200);
  adquire();

  Serial.print("pressure raw:\t");
  Serial.print(press_raw);
  Serial.print("\t pressure mesured:\t");
  Serial.print(get_press());
  Serial.print("\t temperature raw:\t");
  Serial.print(temp_raw);
  Serial.print("\ttemperature mesured:\t");
  Serial.println(get_temp());
  
}



void  adquire()
{
  Wire.beginTransmission(SENSOR_ADDR);  
  
  switch (mode)
  {
      case 0:
        n = Wire.requestFrom(SENSOR_ADDR,2);
        break;
      case 1:
        n =Wire.requestFrom(SENSOR_ADDR,3);
        break;
      case 2:
        n =Wire.requestFrom(SENSOR_ADDR,4);
        break;
      default:
        n =Wire.requestFrom(SENSOR_ADDR,4);
  }

  
  printdb("n of data",1);
  printdb(n,1);


  while(Wire.available())
  {
   
    data[idx] = Wire.read();
    printdb(data[idx],1);
    idx+=1;

  }
  
  Wire.endTransmission();
  process_sensorData(data);
  idx=0;
}


void process_sensorData(byte data[])
{

  if(mode == PARCIAL_TEMP_MODE)
  {
    printdb("aca",1);
    temp_raw = data[2];  
  }
  else
    temp_raw  = (data[2]<<3) + ((data[3]>>5)&0x07);

  printdb(temp_raw,1);

  press_raw = ((data[0]&0x3f)<<8) + data[1];
  
  psv.temp = calculate_temperature(temp_raw);
  psv.press = calculate_pressure(press_raw);
  psv.error = data[0]>>6;

  temp_buff[idx_sample] = psv.temp;
  press_buff[idx_sample] = psv.press;
  idx_sample +=1 ;
  if(idx_sample>=BUFF_SIZE)
    idx_sample = 0;
}


float calculate_pressure(long press)
{
  return 1.25*((press - OS_DIG)/16384.0)*FSS_H20;
}

float calculate_temperature(long temp)
{
  //if(mode==PARCIAL_TEMP_MODE)
  //  return float(temp*(0.7843)-50.0);
  //else
    return float(temp*(0.0977)-50.0);
}

float get_press(bool avr)
{
  if(avr)
  {
    float sum=0.0;
    for(int i=0;i<BUFF_SIZE;i++)
    {
      sum+=press_buff[i];
    }
    return sum/BUFF_SIZE;
  }
  else
    return get_press();
}

float get_press()
{
  return psv.press;
}


float get_temp(bool avr)
{
  if(avr)
  {
    float sum=0.0;
    for(int i=0;i<BUFF_SIZE;i++)
    {
      sum+=temp_buff[i];
    }
    return sum/BUFF_SIZE;
  }
  else
    return get_temp();
}

float get_temp()
{
  return psv.temp;
}

void printdb(int x,bool ln)
{
  #ifdef DEBUG
    if(ln)
      Serial.println(x);
    else
      Serial.print(x);
  #endif
}

void printdb(long x,bool ln)
{
  #ifdef DEBUG
    if(ln)
      Serial.println(x,HEX);
    else
      Serial.print(x,HEX);
  #endif
}


void printdb(char *x,bool ln)
{
  #ifdef DEBUG
    if(ln)
      Serial.println(x);
    else
      Serial.print(x);
  #endif
}

void printdb(byte x,bool ln)
{
  #ifdef DEBUG
    if(ln)
      Serial.println(x,HEX);
    else
      Serial.print(x,HEX);
  #endif
}