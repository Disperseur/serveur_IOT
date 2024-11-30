#include "MKRLiPo2A_LowPower.h"

//define variables
float rawADC;           //unprocessed ADC value
float voltADC;          //ADC converted into voltage
float voltBat;          //calculated voltage on battery

//define the resistor values in the voltage divider
  
int R1 =  330000;       // resistor between battery terminal and SAMD pin PB09
int R2 = 1000000;       // resistor between SAMD pin PB09 and ground

int max_Source_voltage; // upper source voltage for the battery

// define voltage at which battery is full/empty
float batteryFullVoltage = 4.2;   //upper voltage limit for battery
float batteryEmptyVoltage = 3.5;  //lower voltage limit for battery

float batteryCapacity = 2.0;            //set battery capacity in Ah

int new_batt;

void LowPower_Setup(void) {
  analogReference(AR_DEFAULT);      // the upper value of ADC is set to 3.3V
  analogReadResolution(12);         // this will give us 4096 (2^12) levels on the ADC

  //configure BQ24195 PMIC
  PMIC.begin();                                               // start the PMIC I2C connection
  //PMIC.enableBoostMode();                                     // boost battery output to 5V
  
  PMIC.setMinimumSystemVoltage(batteryEmptyVoltage);          // set the minimum battery output to 3.5V
  PMIC.setChargeVoltage(batteryFullVoltage);                  // set battery voltage at full charge

  PMIC.setChargeCurrent(batteryCapacity/2);                   // set battery current to C/2 in amps

  PMIC.enableCharge();                                        // enable charging of battery

  // The formula for calculating the output of a voltage divider is
  // Vout = (Vsource x R2)/(R1 + R2)
  // If we consider that 3.3V is the maximum that can be applied to Vout then the maximum source voltage is calculated as
  max_Source_voltage = (3.3 * (R1 + R2))/R2;
}



void LowPower_calculBattery(void) {
  rawADC = analogRead(ADC_BATTERY);                     //the value obtained directly at the PB09 input pin
  voltADC = rawADC * (3.3/4095.0);                      //convert ADC value to the voltage read at the pin
  voltBat = voltADC * (max_Source_voltage/3.3);         //we cannot use map since it requires int inputs/outputs
  
  new_batt = (voltBat - batteryEmptyVoltage) * (100) / (batteryFullVoltage - batteryEmptyVoltage);    //custom float friendly map function

}




int LowPower_getBattery(void) {
  LowPower_calculBattery();
  return(new_batt);
}




void LowPower_SamdNina_sleep(int millis) {
  WiFi.end();
  LowPower.sleep(millis);
}

