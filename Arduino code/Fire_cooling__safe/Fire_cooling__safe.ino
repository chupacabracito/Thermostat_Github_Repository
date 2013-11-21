/*
  This code turns on the fire when you heat up the thermistor.
  It also turns on LEDs for disgnostics.
  
*/


 
int igniter = A1;
int propane = A2;
int BlueLED = A3; 
int RedLED = A4;
int thermosensor = A0;
int SetpointCool = 24;
int SetpointHeat = 20;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  Serial.begin(115200);

  pinMode(igniter, OUTPUT);  
  pinMode(propane, OUTPUT);
  pinMode(BlueLED, OUTPUT);     
}









void loop() {
  //read serial for setpoint change:
   if (Serial.available()) {
        /* read the most recent byte */
        SetpointCool = Serial.parseInt();
       }
  //done reading for setpoint change
  


    
    //read & print temperature:
    // read the input on analog pin 0:
    int sensorValue = analogRead(thermosensor);
    // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
    float voltage = sensorValue * (5.0 / 1023.0);
    float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset

    // print out the value you read:
    Serial.print("Temperature: ");  Serial.print(temperatureC);
    Serial.print("      SetpointCool: ");  Serial.print(SetpointCool);
    Serial.print("      SetpointHeat: ");  Serial.print(SetpointHeat);
    Serial.print('\n');
    //done reading & displaying temperature




    //set fire if it's warm,
    //light up red LED for heating & blue LED for cooling:
    if(temperatureC > SetpointCool) {
           digitalWrite(BlueLED, HIGH);   // turn the blue LED on
           digitalWrite(RedLED, LOW);  
           digitalWrite(igniter, HIGH);    // turn on igniter
           delay(5000);                     // wait for igniter to heat up
           digitalWrite(propane, HIGH);    // turn on propane
           delay(1000);                    //blow fire for a second before resetting
 
         }  
       else {
              if(temperatureC < SetpointHeat) { 
                     digitalWrite(RedLED, HIGH);   // turn the red LED on
                     digitalWrite(BlueLED, LOW);
                    }
                else {
                       digitalWrite(RedLED, LOW);  
                       digitalWrite(BlueLED, LOW);
                      }
           }  





  
  
    //Turn off fire
       digitalWrite(igniter, LOW);  // turn off igniter
       digitalWrite(propane, LOW);  //turn off propane
  

  
  
  //delay(100);               // wait for a tad
}
