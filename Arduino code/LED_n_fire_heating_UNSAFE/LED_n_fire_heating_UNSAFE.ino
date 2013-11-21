/*
 * runs the BlinkM LED and also turns on fire when temp gets HOT, not cold
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
  
  int igniter = A3;
  int propane = A4;
  pinMode(igniter, OUTPUT);  
  pinMode(propane, OUTPUT);
  boolean FireOn = TRUE;
  
} 

void loop()
{
  bri_val = 50;    //analogRead(bri_pot_pin);    // read the brightness pot
  int sensorValue = analogRead(A0);
    // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
    float voltage = sensorValue * (5.0 / 1023.0);
    float temperature = (voltage - .5)*100;
    
    
    //turn fire on for the first time, if necessary
    if(temperature <= 71) and FireOn == FALSE) { hue_val = 0; } //note: Red is hue of zero.
      else {
            if(temperature > 73) { 
                hue_val = 150; //note: hue for blue
                digitalWrite(Igniter, HIGH);  // turn on igniter
                delay(5000);
                digitalWrite(propane, HIGH);  //turn on propane
              }      
            
            else { hue_val = 16; }   //note: Yellow is hue of 16.
           }  
    
    
        //leave fire on if already on
        
    
    
  //LED - any old color, based on voltage --->  float hue_val = -715 + voltage*1000;     <--------NEED TO FIX SCALING FACTOR  
  Serial.print("Voltage "); Serial.print(voltage);
  Serial.print("      Temperature "); Serial.print(temperature);
  //Serial.print("      brightness "); Serial.print(bri_val); 
  Serial.print("     hue "); Serial.println(hue_val);
  
  // set blinkms with hue & bri, saturation is max
  BlinkM_fadeToHSB( blinkm_addr, hue_val, 255, bri_val );
  
  
  //Turn off fire
       digitalWrite(Igniter, LOW);  // turn off igniter
       delay(5000);
       digitalWrite(propane, LOW);  //turn off propane
  
  delay(50);  // wait a bit because we don't need to go fast
} 
