/*
 My button code
 
based on "DigitalInputPullup". 
It reads a digital input on pin 2 and prints the results to the serial monitor.
 
 The circuit: 
 * Momentary switch attached from pin 2 to ground 
 * Built-in LED on pin 13
 

 */
const int ledPin = 13; 

//debouncing:
int buttonState = LOW;
int lastPin2Val = HIGH;
int lastPin3Val = HIGH;


//long debounceDelay = 50;
//long lastDebounceTime = 0;



void setup(){
  //start serial connection
  Serial.begin(9600);
  //configure pin2 as an input and enable the internal pull-up resistor
  pinMode(ledPin, OUTPUT); 
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);

}




void loop(){
  //read the pushbutton value into a variable
  int Pin2Val = digitalRead(2);
  int Pin3Val = digitalRead(3);


  // If the switch changed, due to noise or pressing:
  if (Pin2Val != lastPin2Val or Pin3Val != lastPin3Val) {

    
    // the pushbutton goes HIGH when it's open and LOW when it's pressed.    
   if (Pin2Val == LOW or Pin3Val == LOW) {digitalWrite(13, HIGH);  } 
   else {digitalWrite(13, LOW); }

  //wait a bit, then re-read button inputs to allow pressing buttons non-simultaneously:
   delay(100);
    int Pin2Val = digitalRead(2);
    int Pin3Val = digitalRead(3);

   if (Pin2Val == LOW and Pin3Val == LOW) { Serial.println("both buttons pushed"); }
   else {
      if (Pin2Val == LOW) { Serial.println("BUTTON 2 pushed"); }
      if (Pin3Val == LOW) { Serial.println("  BUTTON 3 pushed"); }
   }
  

  
  
  }
  
  lastPin2Val = Pin2Val;
  lastPin3Val = Pin3Val;
     
}



