//THERMISTOR/////////////////////////////////////////
// which analog pin to connect
#define THERMISTORPIN A0         
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

int samples[NUMSAMPLES];





//THE SENSOR////////////////////////////////////////
#define aref_voltage 3.3         // we tie 3V to ARef and measure it with a multimeter!
//TMP36 Pin Variables
int tempPin = 2;        //the analog pin the TMP36's Vout (sense) pin is connected to
                        //the resolution is 10 mV / degree centigrade with a
                        //500 mV offset to allow for negative temperatures
int tempReading;        // the analog reading from the sensor
 
 
 
 
/////////////////////////////////////////////////////
void setup(void) {
  // We'll send debugging information via the Serial monitor
  Serial.begin(9600);   
 
  // If you want to set the aref to something other than 5v
  analogReference(EXTERNAL);
}

void loop(void) {
  
  
  
  
  
  
  
  
  
//THERMISTOR/////////////////////////////////////////
  uint8_t i;
  float average;
 
 
  // take N samples in a row, with a slight delay
  for (i=0; i< NUMSAMPLES; i++) {
   samples[i] = analogRead(THERMISTORPIN);
   delay(10);
  }
 
  // average all the samples out
  average = 0;
  for (i=0; i< NUMSAMPLES; i++) {
     average += samples[i];
  }
  average /= NUMSAMPLES;
 
  //Serial.print("Average analog reading "); 
  //Serial.println(average);
 
  // convert the value to resistance
  average = 1023 / average - 1;
  average = SERIESRESISTOR / average;
 // Serial.print("Thermistor resistance "); 
 // Serial.println(average);
 
  float steinhart;
  steinhart = average / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert to C
 
  Serial.print("Thermistor Temp = "); 
  Serial.print(steinhart);
  Serial.print(" *C");
 
  delay(1000);
  
  
  
  
  
  
  
//THE SENSOR///////////////////////////////////////// 
 tempReading = analogRead(tempPin);  
 
  Serial.print("         Sensor Temp = ");
  //Serial.println(tempReading);     // the raw analog reading
 
  // converting that reading to voltage, which is based off the reference voltage
  float voltage = tempReading * aref_voltage;
  voltage /= 1024.0; 
 
  // print out the voltage
 // Serial.print(" - ");
  //Serial.print(voltage); Serial.println(" volts");
 
  // now print out the temperature
  float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
                                               //to degrees ((volatge - 500mV) times 100)
                                               
 float CalibratedSensorTemperature = temperatureC + 5.234;
 
  Serial.print(CalibratedSensorTemperature); Serial.println(" *C");
 
  // now convert to Fahrenheight
  //float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
  //Serial.print(temperatureF); Serial.println(" degrees F");
 
  delay(1000);
 
 
}
