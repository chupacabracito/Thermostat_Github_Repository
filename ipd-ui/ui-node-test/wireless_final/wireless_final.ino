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
#define LEDPINY                6
#define LEDPING                7
#define LEDPINR                8
#define LISTEN_PORT            23 // Telnet protocol

// Create CC3000 instances
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
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
          pinMode(LEDPINY, OUTPUT);
          pinMode(LEDPING, OUTPUT);
          pinMode(LEDPINR, OUTPUT);
          pinMode(ANALOGINPIN, INPUT);          
          
          /*
          // Set up the CC3000, connect to the access point, and get an IP address.
          Serial.println(F("\nInitializing..."));
          if (!cc3000.begin())
          {
            Serial.println(F("Couldn't begin()! Check your wiring?"));
            while(1);
          }
          if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
            Serial.println(F("Failed!"));
            while(1);
          }
          Serial.println(F("Connected!"));
          Serial.println(F("Request DHCP"));
          while (!cc3000.checkDHCP())
          {
            delay(100);
          }  
          while (!displayConnectionDetails()) {
            delay(1000);
          }            
          
  // Start the candy bowl server.
  testServer.begin();
  
  Serial.println(F("Listening for connections..."));          
  */
}

void loop(void) {
 // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
 // data that is being sent to the server  
 /*
 Adafruit_CC3000_ClientRef client = testServer.available(); 
 if (client) {
   */
byte incomingByte = Serial.read();
 
 if (onOff == true) {
   if (incomingByte == 49) {
     onOff = true;
   }
   float sensorValue = analogRead(A0);          
   float voltage = sensorValue * (5.0 / 1023.0);
   int temperature = int(voltage*100) + random(-1,1);    //<------NOTE: NOT ACTUALLY CALIBRATED TEMP.  BUT LOOKS CLOSE.
   char TempAsChar[10];
   dtostrf(temperature,1,0,TempAsChar);
   Serial.print("The current temperature is: " + String(temperature) + " degrees.");

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

