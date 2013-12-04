////////////////////////////////////THERMISTOR_DECLERATIONS
// which analog pin to connect
#define THERMISTORPIN_RADIANT A0   
#define THERMOSTORPIN_AIR A2
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




///////////////////////////////////FIRE_CONTROL_DECLERATIONS
/*
  This code turns on the fire when you heat up the thermistor.
  It also turns on LEDs for disgnostics. 
*/
 
int igniter = 7;
int propane = 8;
int cooler = 9;
int BlueLED = 6;    // actually not used
int YellowLED = 6;  // actually not used
int RedLED = A4;    // actually not used
//int thermosensor = A1;
//int SetpointHeat = 60;
//int SetpointCool;  // maybe make independent later
boolean FireOn = false;
boolean CoolingOn = false;




//////////////////////////////////ACCELEROMETER DECLERATIONS
/*
 ADXL3xx
 
 Reads an Analog Devices ADXL3xx accelerometer and communicates the
 acceleration to the computer.  The pins used are designed to be easily
 compatible with the breakout boards from Sparkfun, available from:
 http://www.sparkfun.com/commerce/categories.php?c=80

 http://www.arduino.cc/en/Tutorial/ADXL3xx

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
  float fahrenheit_rad = steinhart_rad * 9/5 + 32;     // convert to F
 
  Serial.print("Radiant Temp: "); 
  Serial.print(fahrenheit_rad);
  Serial.print(" *F");
  Serial.print('\t');
 
 //////////////////////////////////READ AIR TEMP
 
  // take N samples in a row, with a slight delay
  for (i=0; i< NUMSAMPLES; i++) {
   samples_air[i] = analogRead(THERMOSTORPIN_AIR);
   delay(10);
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
   fahrenheit_air = fahrenheit_rad;
   //delete that when air temp sensor working!
 
  Serial.print("Air Temp (fake): "); 
  Serial.print(fahrenheit_air);
  Serial.print(" *F");
  Serial.print('\t');
 
 
 ////////////////////////////////// calc operative temp
 OperativeTemp = (fahrenheit_rad + fahrenheit_air)/2;

  Serial.print("Operative Temp: "); 
  Serial.print(OperativeTemp);
  Serial.print(" *F");
  Serial.print('\t');





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
  
  float cosine = analogRead(xpin) * (-0.0152) + 4.97;
  
  if (cosine > 1) {
    cosine = 1;}
  if (cosine < (-1)) {
    cosine = -1;}
    
  float arccos = acos(cosine);
  
  int SetpointHeat = arccos * (-10.308) + 91.905;
  
 // Serial.print(arccos);
 //   Serial.print("\t");
     Serial.print("Set temperature: *F");
     Serial.print(SetpointHeat);
     Serial.print('\t');
  

 
  // delay before next reading:
  delay(500);









 ////////////////////////////////// HEATING / COOLING CONTROL


  //read serial for setpoint change:
  // if (Serial.available()) {
        /* read the most recent byte */
  //      SetpointHeat = Serial.parseInt();
  //     }
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



    Serial.print("      Setpoint: ");  Serial.print(SetpointHeat);
    Serial.print("      SetpointCool: ");  Serial.print(SetpointCool);
    Serial.print("      FireOn: ");  Serial.print(FireOn);
    Serial.print("      CoolingOn: ");  Serial.print(CoolingOn);
    Serial.println();

    //done reading & displaying temperature


    ////////////////////////// THE IMPORTANT PART OF HEATING / COOLING:
    //Turn on fire if it's cool, turn on cooling if it's warm.  
    //Turn everything off if temp in spec.
    //light up red LED for heating & blue LED for cooling:
    if(OperativeTemp < SetpointHeat) {  
          // turn off cooling:
          digitalWrite(cooler, LOW);   
          CoolingOn = false;
          digitalWrite(BlueLED, LOW);  

          //turn fire on!
          if(FireOn == false) {
             digitalWrite(igniter, HIGH);    // turn on igniter
             digitalWrite(YellowLED, HIGH);
             delay(5000);                     // wait for igniter to heat up
             digitalWrite(propane, HIGH);    // turn on propane
             delay(100);                    //blow fire for a second before resetting
             FireOn = true;
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
                   digitalWrite(RedLED, LOW);
                   digitalWrite(YellowLED, LOW);   
                    
                   //Turn on cooling:
                   digitalWrite(cooler, HIGH);  
                   CoolingOn = true;          
                   digitalWrite(BlueLED, HIGH);

                 }

                else {       // temperature is juuuuust right
                       //Turn off cooling:
                       digitalWrite(cooler, LOW); 
                       CoolingOn = false;
                       digitalWrite(BlueLED, LOW);

                       //Turn off fire
                       digitalWrite(igniter, LOW);  // turn off igniter
                       digitalWrite(propane, LOW);  //turn off propane
                       FireOn = false;
                       digitalWrite(RedLED, LOW);                       
                       digitalWrite(YellowLED, LOW);    
                      }
           }
  
  delay(100);               // wait for a tad
}




