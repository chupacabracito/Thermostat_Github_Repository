// Libraries
#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>
#include "utility/debug.h"
#include<stdlib.h>

// Define CC3000 chip pins
#define ADAFRUIT_CC3000_IRQ   3
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
#define ANALOGINPIN           A0
#define LISTEN_PORT           23 // Telnet protocol

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

// Xively parameters
//#define WEBSITE  "juandejoya.com"

// Create an instance of the CC3000 server listening on the specified port.
Adafruit_CC3000_Server testServer(LISTEN_PORT);

uint32_t ip;
int prevTemp; 
boolean onOff = true;
void setup(void) { 
          Serial.begin(115200);
          Serial.println(F("Hello, CC3000!\n")); 
          Serial.print("Free RAM: "); Serial.println(getFreeRam(), DEC);
          pinMode(ANALOGINPIN, INPUT);          
}

void loop(void) {
 // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
 // data that is being sent to the server  

byte incomingByte = Serial.read();
Serial.println("incomingByte: " + String(incomingByte));
float sensorValue = analogRead(A0);          
float voltage = sensorValue * (5.0 / 1023.0);
int atemperature, rtemperature; 

if (onOff == true) {

  atemperature = int(voltage*100) + random(10,40);    //<------NOTE: NOT ACTUALLY CALIBRATED TEMP.  BUT LOOKS CLOSE.
  rtemperature = int(voltage*100) + random(40,90);    //<------NOTE: NOT ACTUALLY CALIBRATED TEMP.  BUT LOOKS CLOSE.

  if (incomingByte == 49) {
    onOff = true;
  }
  if (incomingByte == 48) {
    onOff = false; 
   }
   
} else {
  atemperature = int(voltage*100) + random(-10,-40);    //<------NOTE: NOT ACTUALLY CALIBRATED TEMP.  BUT LOOKS CLOSE.
  rtemperature = int(voltage*100) + random(-40,-90);    //<------NOTE: NOT ACTUALLY CALIBRATED TEMP.  BUT LOOKS CLOSE.    
   if (incomingByte == 49) {
     onOff = true;
   }
   if (incomingByte == 48) {
    onOff = false; 
   }     
  

}

int otemperature = atemperature + rtemperature;    //<------NOTE: NOT ACTUALLY CALIBRATED TEMP.  BUT LOOKS CLOSE.
char aTempAsChar[10];
char rTempAsChar[10];
char oTempAsChar[10];
dtostrf(atemperature,1,0,aTempAsChar);
dtostrf(rtemperature,1,0,rTempAsChar);
dtostrf(otemperature,1,0,oTempAsChar);
Serial.println(String(atemperature)+"°,"+String(rtemperature)+"°*"+String(otemperature)+"°");  
delay(500);
}


// From previous code -> save these for later for optimization/config purposes
/*
 if (onOff == true) {
   if (incomingByte == 49) {
     onOff = true;
   }


   if (prevTemp) {
     if (temperature > prevTemp) {
       Serial.println(" It's hotter now.");                                      
       digitalWrite(LEDPINY, LOW);
       digitalWrite(LEDPING, LOW);          
       digitalWrite(LEDPINR, HIGH);                        
     } else if (temperature < prevTemp) {
       Serial.println(" It's colder now.");
       digitalWrite(LEDPINY, LOW);
       digitalWrite(LEDPING, HIGH);          
       digitalWrite(LEDPINR, LOW);                                      
     } else {
       Serial.println(" Consistent as before.");
       digitalWrite(LEDPINY, HIGH);
       digitalWrite(LEDPING, LOW);          
       digitalWrite(LEDPINR, LOW);                                      
     }                  
   }
   prevTemp = temperature;                   

   if (incomingByte == 48) {
    onOff = false; 
   }
 } else {
   if (incomingByte == 49) {
     onOff = true;
   }
   if (incomingByte == 48) {
    onOff = false; 
   }   
   digitalWrite(LEDPINY, LOW);
   digitalWrite(LEDPING, LOW);          
   digitalWrite(LEDPINR, LOW);     
 } 

 delay(500);   
}

// Display connection details like the IP address.
bool displayConnectionDetails(void)
{
  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;
  
  if(!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
  {
    Serial.println(F("Unable to retrieve the IP Address!\r\n"));
    return false;
  }
  else
  {
    Serial.print(F("\nIP Addr: ")); cc3000.printIPdotsRev(ipAddress);
    Serial.print(F("\nNetmask: ")); cc3000.printIPdotsRev(netmask);
    Serial.print(F("\nGateway: ")); cc3000.printIPdotsRev(gateway);
    Serial.print(F("\nDHCPsrv: ")); cc3000.printIPdotsRev(dhcpserv);
    Serial.print(F("\nDNSserv: ")); cc3000.printIPdotsRev(dnsserv);
    Serial.println();
    return true;
  }
}
*/
