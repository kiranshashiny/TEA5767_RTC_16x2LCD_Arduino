// Programming TEA5767 Radio for different time of the day.

// This is for ESP8266 and the processor communicates to the
// Wireless Router in the house and gets the current date/time using NTP.
// Based on that it plays the designated radio frequency on the TEA5767
// connected over the SDA and SCL pins.

// Connect the SDA and SCL to appropriate pins on the I2C pins on ESP8266


#include <Wire.h>
#include <TEA5767Radio.h>

#include <NTPClient.h>
#include <ESP8266WiFi.h>
//#include <WiFi.h> // for WiFi shield
//#include <WiFi101.h> // for WiFi 101 shield or MKR1000
#include <WiFiUdp.h>
WiFiUDP ntpUDP;

// This is for the IST ( Indian Standard Time)

// Note the 19800 = 5.5 hrs X 3600 in the third argument.

NTPClient timeClient(ntpUDP,"europe.pool.ntp.org", 19800, 60000);

TEA5767Radio radio = TEA5767Radio();
float str[] = {91.1, 91.9, 92.7,  93.5, 94.3,
                  95,   98.3, 100.1, 101.3,102.9,
                  104, 106.4};
void setup()
{ 
  Wire.begin();
  Serial.begin(9600);
  radio.setFrequency(104); // Set up first frequency.

  WiFi.begin("NETGEAR84", "XXXXXXXX" );

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();

}
//
//
// Function that will be woken up every minute and reads the current time.
// If time is within programmed limits - change to the appropriate radio station.
//
//
//
void GetTime_ProgramRadio(){

  
  timeClient.update();
  Serial.println("");
  Serial.println(timeClient.getFormattedTime());

  String formattedDate = timeClient.getFormattedTime();
  Serial.print ("Current Time is " );
  Serial.println ( formattedDate);

  // Extract time
  int splitT = formattedDate.indexOf(":");
  String hour = formattedDate.substring(0, splitT);
  int    hourInt = atoi( hour.c_str());
  //Serial.print("Current Hour : ");
  //Serial.print (hourInt);
  
  String minSecs = formattedDate.substring(splitT+1 , formattedDate.length());

  splitT = minSecs.indexOf(":");
  String min = minSecs.substring(0, splitT);
  int    minInt = atoi(min.c_str());
  
  //Serial.print (", Min = ");
  //Serial.println ( minInt );

  delay(1000*60);
  
  switch (hourInt ) {
       case 7:        
          Serial.println (" 7 AM , Playing 94.3");    
          radio.setFrequency(94.3); // pick your own frequency
          break;
          
       case 8:
          Serial.println ("8 AM Playing 100.1");    
          radio.setFrequency(100.1 ); // pick your own frequency
          break;

        default :
          Serial.println ("default 94.3 ");    
          radio.setFrequency(94.3); // pick your own frequency
          break;          
  }
  
}
  

void loop()
{
  
   GetTime_ProgramRadio();
    
}


