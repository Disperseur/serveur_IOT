// Toujours brancher la batterie avant de brancher le câble usb dans le pc.


#pragma once


#include <BQ24195.h>
#include <WiFiNINA.h>
#include "ArduinoLowPower.h"

//define variables
extern float rawADC;           //unprocessed ADC value
extern float voltADC;          //ADC converted into voltage
extern float voltBat;          //calculated voltage on battery

//define the resistor values in the voltage divider
  
extern int R1;       // resistor between battery terminal and SAMD pin PB09
extern int R2;       // resistor between SAMD pin PB09 and ground

extern int max_Source_voltage; // upper source voltage for the battery

// define voltage at which battery is full/empty
extern float batteryFullVoltage;   //upper voltage limit for battery
extern float batteryEmptyVoltage;  //lower voltage limit for battery

extern float batteryCapacity;            //set battery capacity in Ah

extern int new_batt;

void LowPower_Setup(void);
void LowPower_calculBattery(void);
int LowPower_getBattery(void);

void LowPower_SamdNina_sleep(int millis);



