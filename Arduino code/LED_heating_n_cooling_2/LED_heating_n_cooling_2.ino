/*
 * BlinkColorFader -- Example of how to select color & brightness
 *                    with two pots
 * 
 * For more info on how to use pots and analog inputs see:
 *  http://www.arduino.cc/en/Tutorial/AnalogInput
 *
 * BlinkM connections to Arduino
 * PWR - -- gnd -- black -- Gnd
 * PWR + -- +5V -- red   -- 5V
 * I2C d -- SDA -- green -- Analog In 4
 * I2C c -- SCK -- blue  -- Analog In 5
 *
 * Note: This sketch sends to the I2C "broadcast" address of 0, 
 *       so all BlinkMs on the I2C bus will respond.
 */

#include "Wire.h"
#include "BlinkM_funcs.h"

#define blinkm_addr 0x00

byte bri_val;
byte hue_val;

void setup()
{
  BlinkM_beginWithPower();
  BlinkM_stopScript(blinkm_addr);  // turn off startup script
  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
} 

void loop()
{
  bri_val = 50;    //analogRead(bri_pot_pin);    // read the brightness pot
  int sensorValue = analogRead(A0);
    // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
    float voltage = sensorValue * (5.0 / 1023.0);
    float temperature = (voltage - .5)*100;
    
    if(temperature <= 71) { hue_val = 0; } //note: Red is hue of zero.
      else {
            if(temperature > 73) { hue_val = 150; }    //note: hue for blue  
            else { hue_val = 16; }   //note: Yellow is hue of 16.
           }  
    
    //any old color, based on voltage --->  float hue_val = -715 + voltage*1000;     <--------NEED TO FIX SCALING FACTOR  
  Serial.print("Voltage "); Serial.print(voltage);
  Serial.print("      Temperature "); Serial.print(temperature);
  //Serial.print("      brightness "); Serial.print(bri_val); 
  Serial.print("     hue "); Serial.println(hue_val);
  
  // set blinkms with hue & bri, saturation is max
  BlinkM_fadeToHSB( blinkm_addr, hue_val, 255, bri_val );
  
  delay(50);  // wait a bit because we don't need to go fast
} 
