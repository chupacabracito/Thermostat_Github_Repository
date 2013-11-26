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


void setup()
{
  // initialize the serial communications:
  Serial.begin(9600);
}



void loop()
{

  
  // print the sensor values:
  Serial.print(analogRead(xpin));
  // print a tab between values:
  Serial.print("\t");
  //Serial.print(analogRead(ypin));
  // print a tab between values:
 // Serial.print("\t");
  //Serial.print(analogRead(zpin));
  //print a linefeed:
  
  float cosine = analogRead(xpin) * (-0.0152) + 4.97;
  float arccos = acos(cosine);
 
  Serial.print(cosine);
    Serial.print("\t");
    
  

  
  Serial.println();
  // delay before next reading:
  delay(500);
}

