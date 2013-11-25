//////////////////////////////////////////////THERMISTOR
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
 
void setup(void) {
  Serial.begin(9600);
  analogReference(EXTERNAL);
}

void loop(void) {
  uint8_t i;
  float average_rad;
  float average_air;
  float operating;
 
 ////////////////////////////////////RADIANT 
 
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
  steinhart_rad = average_rad / THERMISTORNOMINAL_RADIANT;     // (R/Ro)
  steinhart_rad = log(steinhart_rad);                  // ln(R/Ro)
  steinhart_rad /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart_rad += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart_rad = 1.0 / steinhart_rad;                 // Invert
  steinhart_rad -= 273.15;                         // convert to C
  float fahrenheit_rad = steinhart_rad * 9/5 + 32;     // convert to F
 
  Serial.print("Radiant Temperature "); 
  Serial.print(fahrenheit_rad);
  Serial.print(" *F");
  Serial.print('\t');
 
 //////////////////////////////////AIR
 
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
  float fahrenheit_air = steinhart_air * 9/5 + 32;     // convert to F
 
  Serial.print("Air Temperature "); 
  Serial.print(fahrenheit_air);
  Serial.print(" *F");
  Serial.print('\t');
 
 
 //////////////////////////////OPERATING
 operating = (fahrenheit_rad + fahrenheit_air)/2;
 
 
 
 
 
 
  delay(100);
}
