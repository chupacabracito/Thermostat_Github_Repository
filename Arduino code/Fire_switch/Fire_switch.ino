/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;
int FireSwitch = 7;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);  
  pinMode(FireSwitch, OUTPUT);    
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(FireSwitch, HIGH);  
  delay(500);               // wait for a second
  digitalWrite(led, LOW);  
  digitalWrite(FireSwitch, LOW);   
  delay(200);               // wait for a second
}


