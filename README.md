# DLVR_PressureSensor
Lib for DLVR pressure sensor with arduino. Still on test. For sensor details please refer to the [datasheet](https://www.mouser.cl/datasheet/2/18/DS-0300_Rev_E-1628655.pdf).

### Tested setup:
  - MCU:ESP32-wroom.
  - SENSOR: DVLR-L30D NI3F
  - PlataformIO

## Sensor configuration 

You can set four parameters in this sensors. This parameters **aren't programables**, and come defined from factory, so you have to be carefull when you order them.
You can refer to the table 5 of the datasheet for more details.
Parameters:
- Coating: Type of coating
- Interface: Type of serial comunication(SPI or I2C) and address
- Supply voltage: 3.3V or 5V
- Speed/power: Operation mode(F, N, L,S)

We are using a sensor with the code **NI3F**: No coating, I2C with address fixed at 0x28, 3.3V voltage supply and logic and fast mode.

This lib acctualy work for the fast mode I2C device. 


## Fast mode

Fast mode is a **free running mode** (page 5 of the datasheet). This mean the sensor is continuously doing convertion and oputing the data. It take pressure samples continuously and add a Sprecial ADC cycle each 225 interval to messure the temperature and Zero reference to internal calibration.

## TODO

- test with arduino ide.
- add comment to all files.
- add automatic sampling with timers.
- add new modes.
- add SPI version.




