////////////////////////////////////THERMISTOR_DECLERATIONS
// which analog pin to connect
#define THERMISTORPIN_RADIANT A1   
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
 
int igniter = 6;
int propane = 7;
int BlueLED = 13; 
int RedLED = A4;  // actually not used
//int thermosensor = A1;
int SetpointCool = 24;
int SetpointHeat = 20;
boolean FireOn = false;





 
void setup(void) {
    Serial.begin(115200);
 
  
  pinMode(igniter, OUTPUT);  
  pinMode(propane, OUTPUT);
  pinMode(BlueLED, OUTPUT);  //Fire Control Setup
}











void loop(void) {
  uint8_t i;
  float average_rad;
  float average_air;
  float operating;
 
 ///////////////////////////////////////////////RADIANT 
 
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
 
  Serial.print("Radiant Temperature: "); 
  Serial.print(fahrenheit_rad);
  Serial.print(" *F");
  Serial.print('\t');
 
 //////////////////////////////////AIR
 
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
 
  Serial.print("Air Temperature: "); 
  Serial.print(fahrenheit_air);
  Serial.print(" *F");
  Serial.print('\t');
 
 
 //////////////////////////////////OPERATING
 operating = (fahrenheit_rad + fahrenheit_air)/2;
   Serial.println(operating);
  
  delay(100);






 //////////////////////////////////FIRE_CONTROL


  //read serial for setpoint change:
   if (Serial.available()) {
        /* read the most recent byte */
        SetpointCool = Serial.parseInt();
       }
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
    Serial.print("      SetpointCool: ");  Serial.print(SetpointCool);
    Serial.print("      SetpointHeat: ");  Serial.print(SetpointHeat);
    Serial.print("      FireOn: ");  Serial.print(FireOn);
    Serial.print('\t');
    //Serial.print('\n');
    //done reading & displaying temperature



    //set fire if it's warm,
    //light up red LED for heating & blue LED for cooling:
    if(fahrenheit_rad > SetpointCool) {
          digitalWrite(BlueLED, HIGH);   // turn the blue LED on
          digitalWrite(RedLED, LOW);  

          //turn fire on!
          if(FireOn == false) {
             digitalWrite(igniter, HIGH);    // turn on igniter
             delay(5000);                     // wait for igniter to heat up
             digitalWrite(propane, HIGH);    // turn on propane
             delay(100);                    //blow fire for a second before resetting
             FireOn = true;
            }
            else{}
           
         }  
       else {
              if(fahrenheit_rad < SetpointHeat) { 
                   digitalWrite(RedLED, HIGH);   // turn the red LED on
                   digitalWrite(BlueLED, LOW);

                   //Turn off fire
                   digitalWrite(igniter, LOW);  // turn off igniter
                   digitalWrite(propane, LOW);  //turn off propane
                   FireOn = false;
                  }

                else {
                       digitalWrite(RedLED, LOW);  
                       digitalWrite(BlueLED, LOW);

                       //Turn off fire
                       digitalWrite(igniter, LOW);  // turn off igniter
                       digitalWrite(propane, LOW);  //turn off propane
                       FireOn = false;
                      }
           }
  
  delay(100);               // wait for a tad
}
