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
#define VERSION    "VMM - v.0.0.0.b.1"

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
#include <DLVR.h>        // Necesary for the pressure sensor
//#include <ArduinoJson.h> // Lib to code and decode  Json format on arduino


#define DEBUG

DLVR press_sensor;


void setup()
{ 
  press_sensor.begin();
  Serial.begin(9600);
  Serial.println("starting");
}

void loop()
{
  delay(200);
  press_sensor.acquire();

  Serial.print("\t pressure mesured:\t");
  Serial.print(press_sensor.get_press());
  Serial.print("\ttemperature mesured:\t");
  Serial.println(press_sensor.get_temp());
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