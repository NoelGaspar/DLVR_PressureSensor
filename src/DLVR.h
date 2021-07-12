/*

Arduino lib for DLVR sensor.




Author: WAC@IOWLABS.

*/
#ifndef DLVR_H
#define DLVR_H

// libs
#include <Arduino.h>
#ifndef TwoWire_h
  #include <Wire.h>
#endif

//-----------
// Definitions
//-----------
// Mode of data adquisition page 8 figure 3
#define PRESSURE_MODE     0 // read pressure 
#define PARTIAL_TEMP_MODE 1 // read pressure and partial temperature
#define ALL_FRAME_MODE    2 // read pressure and temperature

// Sensor default addr
#define SENSOR_ADDR  0x28 // please reffer to page 12 secction "how to order"
// Sensor parameters
#define FSS_H20 60 // from datasheet page 1
#define OS_DIG  8192 // from datasheet page 3
// buffer size to average samples
#define BUFF_SIZE 10


class DLVR
{
  public:
    DLVR(uint8_t mode);
    DLVR();
    void begin();
    void acquire();
    float get_press(bool avr);
    float get_press();
    float get_temp(bool avr);
    float get_temp();

  private:
    struct pressureSensorValues{
      float temp;
      float press;
      float temp_avg;
      float press_avg;
      uint8_t error;
    };
    pressureSensorValues _psv =  {0.0,0.0,0.0,0.0,0};

    uint8_t _mode = ALL_FRAME_MODE;
    byte _data[4];

    int _temp_raw = 0;
    int _press_raw= 0;
    int _idx_sample=0;
    int _idx=0;

    float _temp_buff[BUFF_SIZE];
    float _press_buff[BUFF_SIZE];
    
    void process_sensorData(byte data[]);
    float calculate_temperature(long temp);
    float calculate_pressure(long press);


};




#endif