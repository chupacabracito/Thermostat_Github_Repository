//MERGED CODE
// NOTE: Validate added pins just to make sure things are not in conflict.
// NOTE: Check server.js and change the portName for your local system. 
////////////////////////////////////WIRELESS_COMMUNICATION_LIBRARIES
#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>
#include "utility/debug.h"
#include<stdlib.h>

////////////////////////////////////THERMISTOR_DECLERATIONS
// which analog pin to connect
#define THERMISTORPIN_RADIANT A0   
#define THERMISTORPIN_AIR A2
// resistance at 25 degrees C
#define THERMISTORNOMINAL 10000      
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25   
// how many samples to take and average, more takes longer
// but is more 'smooth'
#define NUMSAMPLES 5
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3950
// the value of the 'other' resistor
#define SERIESRESISTOR 10000    
 
int samples_rad [NUMSAMPLES];
int samples_air [NUMSAMPLES];

// From radiant.ino code for wireless communication
// Define CC3000 chip pins
#define ADAFRUIT_CC3000_IRQ   3 
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
#define ANALOGINPIN           A0
#define LISTEN_PORT           23 // Telnet protocol

//MERGE CODE
///////////////////////////////////WIRELESS_COMMUNICATION_DECLARATION
// Create CC3000 instances
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, 
                                         ADAFRUIT_CC3000_IRQ, 
                                         ADAFRUIT_CC3000_VBAT,
                                         SPI_CLOCK_DIV2); // you can change this clock speed

// WLAN parameters
#define WLAN_SSID       "EECS"
#define WLAN_PASS       "Thequickbrown"
// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WEP

// Create an instance of the CC3000 server listening on the specified port.
// Commented out for the time being, not necessary to the current code.
//Adafruit_CC3000_Server testServer(LISTEN_PORT);
//uint32_t ip;
//boolean onOff = true;


///////////////////////////////////FIRE_CONTROL_DECLERATIONS
/*
  This code turns on the fire when you heat up the thermistor.
  It also turns on LEDs for disgnostics. 
*/
 
int igniter = 7;
int propane = 8;
int cooler = 9;
int BlueLED = 6;    // actually not used
int YellowLED = 2;  // actually not used
int RedLED = 4;    // actually not used
//int thermosensor = A1;
int SetpointHeat = 60;
int SetpointCool;  // maybe make independent later
boolean FireOn = false;
boolean CoolingOn = false;
float arccos = 0;
int KnobSetpointPrevious = 0; 
int cool = 0;
int fire = 0;


//////////////////////////////////DEALING WITH BYTESTREAM
boolean isTemp = false;
int tempChange = 0;

//////////////////////////////////ACCELEROMETER DECLERATIONS
/*
 ADXL3xx
 The circuit:
 analog 0: accelerometer self test
 analog 1: z-axis
 analog 2: y-axis
 analog 3: x-axis

*/

// these constants describe the pins. They won't change:
const int xpin = A3;                  // x-axis of the accelerometer
//const int ypin = A2;                  // y-axis
//const int zpin = A1;                  // z-axis (only on 3-axis models)


#define acosf acos;





 
void setup(void) {
    Serial.begin(115200);
 
 
    
 
  
  pinMode(igniter, OUTPUT);  
  pinMode(propane, OUTPUT);
  pinMode(cooler, OUTPUT);  
  pinMode(YellowLED, OUTPUT);  
  pinMode(RedLED, OUTPUT);
  pinMode(BlueLED, OUTPUT);  
  
  
  
  
}











void loop(void) {
  uint8_t i;
  float average_rad;
  float average_air;
  float OperativeTemp;
 
 ///////////////////////////////////////////////READ RADIANT TEMP 
 
  // take N samples in a row, with a slight delay
  for (i=0; i< NUMSAMPLES; i++) {
   samples_rad[i] = analogRead(THERMISTORPIN_RADIANT);
   delay(10);
  }
 
  // average all the samples out
  average_rad = 0;
  for (i=0; i< NUMSAMPLES; i++) {
     average_rad += samples_rad[i];
  }
  average_rad /= NUMSAMPLES;
 
 // Serial.print("Average analog reading "); 
 // Serial.println(average_rad);
 
  // convert the value to resistance
  average_rad = 1023 / average_rad - 1;
  average_rad = SERIESRESISTOR / average_rad;
// Serial.print("Thermistor resistance "); 
//  Serial.println(average_radiant);
 
  float steinhart_rad;
  steinhart_rad = average_rad / THERMISTORNOMINAL;     // (R/Ro)
  steinhart_rad = log(steinhart_rad);                  // ln(R/Ro)
  steinhart_rad /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart_rad += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart_rad = 1.0 / steinhart_rad;                 // Invert
  steinhart_rad -= 273.15;                         // convert to C
  float fahrenheit_rad = steinhart_rad * 9/5 + 20;     // convert to F
 
  /*
  Serial.print("Radiant Temp: "); 
  Serial.print(fahrenheit_rad);
  Serial.print(" *F");
  Serial.print('\t');
  */
  
 //////////////////////////////////READ AIR TEMP
  //////////////////////////////////READ AIR TEMP

// take N samples in a row, with a slight delay
  for (i=0; i< NUMSAMPLES; i++) {
   samples_air[i] = analogRead(THERMISTORPIN_AIR);
   delay(10);
  }
 
  // average all the samples out
  average_air = 0;
  for (i=0; i< NUMSAMPLES; i++) {
     average_air += samples_air[i];
  }
  average_air /= NUMSAMPLES;
 // Serial.print("Average analog reading "); 
 // Serial.println(average_air);
 
     //read & print temperature from 3-pinthermosensor:
        // read the input on analog pin 0:
                  //int sensorValue = analogRead(thermosensor);
        // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
        float voltage = average_air * (5.0 / 1023.0);
        float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
        float fahrenheit_air = temperatureC * 9/5 + 27;
        // print out the value you read:
        

 /*
  // take N samples in a row, with a slight delay
  for (i=0; i< NUMSAMPLES; i++) {
   samples_air[i] = analogRead(THERMOSTORPIN_AIR);
   //delay(10);
  }
 
  // average all the samples out
  average_air = 0;
  for (i=0; i< NUMSAMPLES; i++) {
     average_air += samples_air[i];
  }
  average_air /= NUMSAMPLES;
 
 // Serial.print("Average analog reading "); 
 // Serial.println(average);
 
  // convert the value to resistance
  average_air = 1023 / average_air - 1;
  average_air = SERIESRESISTOR / average_air;
 // Serial.print("Thermistor resistance "); 
 // Serial.println(average_air);
 
  float steinhart_air;
  steinhart_air = average_air / THERMISTORNOMINAL;     // (R/Ro)
  steinhart_air = log(steinhart_air);                  // ln(R/Ro)
  steinhart_air /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart_air += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart_air = 1.0 / steinhart_air;                 // Invert
  steinhart_air -= 273.15;                         // convert to C
  float fahrenheit_air = fahrenheit_air * 9/5 + 32;     // convert to F
 
  //Since hardware for air temp sensor not working yet, just set equal to radiant temp:
   fahrenheit_air = fahrenheit_rad + 2;
   //delete that when air temp sensor working!
 
 
  //Serial.print("Air Temp (fake): "); 
  //Serial.print(fahrenheit_air);
  //Serial.print(" *F");
  //Serial.print('\t');
 */
 
 ////////////////////////////////// calc operative temp
 OperativeTemp = (fahrenheit_rad + fahrenheit_air)/2;

  //Serial.print("Operative Temp: "); 
  //Serial.print(OperativeTemp);
  //Serial.print(" *F");
  //Serial.print('\t');





////////////////////////////////////////ACCELEROMETER

 // print the sensor values:
 // Serial.print(analogRead(xpin));
  // print a tab between values:
 // Serial.print("\t");
  //Serial.print(analogRead(ypin));
  // print a tab between values:
 // Serial.print("\t");
  //Serial.print(analogRead(zpin));
  //print a linefeed:
  /*
  float cosine = analogRead(xpin) * (-0.0152) + 4.97;
  
  if (cosine > 1) {
    cosine = 1;}
  if (cosine < (-1)) {
    cosine = -1;}
  
  arccos = acos(cosine);
  int KnobSetpoint = arccos * (-10.308) + 91.905;
  /*
  Serial.print("knobSetpoint: "); 
  Serial.print(KnobSetpoint);
  Serial.print("    "); 
  Serial.print("knobSetPrev: "); 
  Serial.print(KnobSetpointPrevious);
  Serial.print("    "); 
  */

  
  //Knob sets temp setpoint IF THE KNOB POSITION HAS CHANGED: 
  /*
  if (abs(KnobSetpoint - KnobSetpointPrevious) > 1) {
          SetpointHeat = KnobSetpoint;    
          KnobSetpointPrevious = KnobSetpoint;
          }
  else {}
    */   
  //done setting temp setpoint
  
  
 // Serial.print(arccos);
 //   Serial.print("\t");
 //   Serial.print("Set temperature: *F");
 //   Serial.print(SetpointHeat);
 //   Serial.print('\t');
 // delay before next reading:
 //delay(500);









 ////////////////////////////////// HEATING / COOLING CONTROL

byte incomingByte = Serial.read();
  //Serial.println("incomingByte: " + String(incomingByte)); 
  if (incomingByte == 65) {
    SetpointHeat = OperativeTemp - 10;
  } else if (incomingByte == 66) {
    SetpointHeat = OperativeTemp + 10;    
  } else if (incomingByte == 67) {
    SetpointCool = OperativeTemp + 10;        
  } else if (incomingByte == 68) {
    SetpointCool = OperativeTemp - 10;            
  }  else if (incomingByte != 255) {
    if (isTemp == true) {
      if (incomingByte == 48) {
        tempChange += 0;
      } else if (incomingByte == 49) {
        tempChange += 1;        
      } else if (incomingByte == 50) {
        tempChange += 2;        
      } else if (incomingByte == 51) {
        tempChange += 3;
      } else if (incomingByte == 52) {
        tempChange += 4;        
      } else if (incomingByte == 53) {
        tempChange += 5;
      } else if (incomingByte == 54) {
        tempChange += 6;        
      } else if (incomingByte == 55) {
        tempChange += 7;
      } else if (incomingByte == 56) {
        tempChange += 8;        
      } else if (incomingByte == 57) {
        tempChange += 9;  
      }      
      
      //Serial.println("final temp change: " + String(tempChange));
      // Set temperature change here <- ask Jer about this
      SetpointHeat = tempChange;
      isTemp = false;
    } else {
      if (incomingByte == 48) {
        tempChange = 0;
      } else if (incomingByte == 49) {
        tempChange = 1;        
      } else if (incomingByte == 50) {
        tempChange = 2;        
      } else if (incomingByte == 51) {
        tempChange = 3;
      } else if (incomingByte == 52) {
        tempChange = 4;        
      } else if (incomingByte == 53) {
        tempChange = 5;
      } else if (incomingByte == 54) {
        tempChange = 6;        
      } else if (incomingByte == 55) {
        tempChange = 7;
      } else if (incomingByte == 56) {
        tempChange = 8;        
      } else if (incomingByte == 57) {
        tempChange = 9;  
      }      
      tempChange *= 10;
      //Serial.println("temp change in tens: " + String(tempChange));
      isTemp = true;
    }
  }
  SetpointCool = SetpointHeat + 5;  
  //Serial.print(String(FireOn)+" ");  
  //Serial.println(String(CoolingOn)+" ");  
  
  //read serial for setpoint change:
  /*
   if (Serial.available()) {
        SetpointHeat = Serial.parseInt();
       }
       int SetpointCool = SetpointHeat + 5;  // resetting just in case
  //done reading for setpoint change
  
    
    //read & print temperature from 3-pinthermosensor:
        // read the input on analog pin 0:
        //int sensorValue = analogRead(thermosensor);
        // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
        //float voltage = sensorValue * (5.0 / 1023.0);
        //float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
        //float temperatureF = temperatureC * 9/5 + 32;
        // print out the value you read:
        // Serial.print("Temperature: ");  Serial.print(temperatureF);


    /*
    Serial.print("      SetpointHeat: ");  Serial.print(SetpointHeat);
    Serial.print("      SetpointCool: ");  Serial.print(SetpointCool);
    Serial.print("      FireOn: ");  Serial.print(FireOn);
    Serial.print("      CoolingOn: ");  Serial.print(CoolingOn);
    Serial.println();
    */

    //done reading & displaying temperature

    ////////////////////////// THE IMPORTANT PART OF HEATING / COOLING:
    //Turn on fire if it's cool, turn on cooling if it's warm.  
    //Turn everything off if temp in spec.
    //light up red LED for heating & blue LED for cooling:
    if(OperativeTemp < SetpointHeat) {  
          // turn off cooling:
          digitalWrite(cooler, LOW);   
          CoolingOn = false;
          cool = 0;
          digitalWrite(BlueLED, LOW);  

          //turn fire on!
          if(FireOn == false) {
           
             FireOn = true;
             fire = 1;            
            Serial.print(String(int(fahrenheit_rad)) + "°/");//int(fahrenheit_rad-15)) + "°F ");
            Serial.print(String(int(fahrenheit_air)) + "°/");//int(fahrenheit_air-15)) + "°F ");
            Serial.print(String(int(OperativeTemp))+"°/");
            Serial.print(String(SetpointHeat) + "°/");
            Serial.print(String(SetpointCool) + "°/");  
            Serial.print(String(int(fire)) + "/");
            Serial.println(String(int(cool)));              
             digitalWrite(igniter, HIGH);    // turn on igniter
             digitalWrite(YellowLED, HIGH);             
             delay(5);                     // wait for igniter to heat up
             digitalWrite(propane, HIGH);    // turn on propane
             delay(100);                    //blow fire for a second before resetting

             digitalWrite(RedLED, HIGH);

            }
            else{}
           
         }  
       else {
              if(OperativeTemp > SetpointCool) { 
                   //Turn off fire:
                   digitalWrite(igniter, LOW);  // turn off igniter
                   digitalWrite(propane, LOW);  //turn off propane
                   FireOn = false;
                   fire = 0;
                   digitalWrite(RedLED, LOW);
                   digitalWrite(YellowLED, LOW);   
                    
                   //Turn on cooling:
                   digitalWrite(cooler, HIGH);  
                   CoolingOn = true;          
                   cool = 1;
                   digitalWrite(BlueLED, HIGH);

                 }

                else {       // temperature is juuuuust right
                       //Turn off cooling:
                       digitalWrite(cooler, LOW); 
                       CoolingOn = false;
                       cool = 0;
                       digitalWrite(BlueLED, LOW);

                       //Turn off fire
                       digitalWrite(igniter, LOW);  // turn off igniter
                       digitalWrite(propane, LOW);  //turn off propane
                       FireOn = false;
                       fire = 0;
                       digitalWrite(RedLED, LOW);                       
                       digitalWrite(YellowLED, LOW);    
                      }
           }
  
  Serial.print(String(int(fahrenheit_rad)) + "°/");//int(fahrenheit_rad-15)) + "°F ");
  Serial.print(String(int(fahrenheit_air)) + "°/");//int(fahrenheit_air-15)) + "°F ");
  Serial.print(String(int(OperativeTemp))+"°/");
  Serial.print(String(SetpointHeat) + "°/");
  Serial.print(String(SetpointCool) + "°/");  
  Serial.print(String(int(fire)) + "/");
  Serial.println(String(int(cool)));
  
  delay(100);               // wait for a tad
}




