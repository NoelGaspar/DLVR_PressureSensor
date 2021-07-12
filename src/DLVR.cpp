/*
  Morse.cpp - Library for flashing Morse code.
  Author: WAC@IOWLABS.
  Released into the public domain.
*/

#include "DLVR.h"

DLVR::DLVR(uint8_t mode)
{
  _mode = mode;
}

DLVR::DLVR()
{
  _mode = ALL_FRAME_MODE;  
}

void DLVR::begin()
{
  Wire.begin();
}

void DLVR::acquire()
{
  Wire.beginTransmission(SENSOR_ADDR);  
  
  switch (_mode)
  {
      case 0:
        Wire.requestFrom(SENSOR_ADDR,2);
        break;
      case 1:
        Wire.requestFrom(SENSOR_ADDR,3);
        break;
      case 2:
        Wire.requestFrom(SENSOR_ADDR,4);
        break;
      default:
        Wire.requestFrom(SENSOR_ADDR,4);
  }

  while(Wire.available())
  {
   
    _data[_idx] = Wire.read();
    _idx+=1;
  }
  
  Wire.endTransmission();
  process_sensorData(_data);
  _idx=0;
}


float DLVR::get_press(bool avr)
{
  if(avr)
  {
    float sum=0.0;
    for(int i=0;i<BUFF_SIZE;i++)
    {
      sum+=_press_buff[i];
    }
    return sum/BUFF_SIZE;
  }
  else
    return get_press();
}

float DLVR::get_press()
{
  return _psv.press;
}


float DLVR::get_temp(bool avr)
{
  if(avr)
  {
    float sum=0.0;
    for(int i=0;i<BUFF_SIZE;i++)
    {
      sum+=_temp_buff[i];
    }
    return sum/BUFF_SIZE;
  }
  else
    return get_temp();
}

float DLVR::get_temp()
{
  return _psv.temp;
}

void DLVR::process_sensorData(byte data[])
{

  if(_mode == PARTIAL_TEMP_MODE)
  {
    _temp_raw = data[2];  
  }
  else
    _temp_raw  = (data[2]<<3) + ((data[3]>>5)&0x07);

  _press_raw = ((data[0]&0x3f)<<8) + data[1];
  
  _psv.temp = calculate_temperature(_temp_raw);
  _psv.press = calculate_pressure(_press_raw);
  _psv.error = data[0]>>6;

  _temp_buff[_idx_sample] = _psv.temp;
  _press_buff[_idx_sample] = _psv.press;
  _idx_sample +=1 ;
  if(_idx_sample>=BUFF_SIZE)
    _idx_sample = 0;
}


float DLVR::calculate_pressure(long press)
{
  return 1.25*((press - OS_DIG)/16384.0)*FSS_H20;
}

float DLVR::calculate_temperature(long temp)
{
  //if(mode==PARTIAL_TEMP_MODE)
  //  return float(temp*(0.7843)-50.0);
  //else
    return float(temp*(0.0977)-50.0);
}