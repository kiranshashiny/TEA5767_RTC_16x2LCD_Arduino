#include "Wire.h"
#include <LiquidCrystal.h>
#include <TEA5767Radio.h>

#define DS3231_I2C_ADDRESS 0x68

const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to

TEA5767Radio radio = TEA5767Radio();

int   sensorValue = 0;        // value read from the pot
float FM_Freq = 0;        // value output to the PWM (analog out)

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

float mymap(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


// Convert normal decimal numbers to binary coded decimal

byte decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}
void setup()
{
  Wire.begin();
  Serial.begin(9600);
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("WH Inventors Stn");
  
  // set the initial time here:
  // DS3231 seconds, minutes, hours, day, date, month, year
   setDS3231time(30,36,15,7,18,05,19);
}
void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year)
{
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}
void readDS3231time(byte *second,
byte *minute,
byte *hour,
byte *dayOfWeek,
byte *dayOfMonth,
byte *month,
byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}
void displayTime()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  // send it to the serial monitor
  Serial.print(hour, DEC);
  // convert the byte variable to a decimal number when displayed
  Serial.print(":");
  if (minute<10)
  {
    Serial.print("0");
  }
  Serial.print(minute, DEC);
  Serial.print(":");
  if (second<10)
  {
    Serial.print("0");
  }
  Serial.print(second, DEC);
  Serial.print(" ");
  Serial.print(dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.print(year, DEC);
  Serial.print(" Day of week: ");
  switch(dayOfWeek){
  case 1:
    Serial.println("Sunday");
    break;
  case 2:
    Serial.println("Monday");
    break;
  case 3:
    Serial.println("Tuesday");
    break;
  case 4:
    Serial.println("Wednesday");
    break;
  case 5:
    Serial.println("Thursday");
    break;
  case 6:
    Serial.println("Friday");
    break;
  case 7:
    Serial.println("Saturday");
    break;
  }
}
void loop()
{
  displayTime(); // display the real-time clock data on the Serial Monitor,
  delay(1000); // every second
  lcd.setCursor(0, 1);
  String str = "FM Radio Station";
  //lcd.print(millis() / 1000);
  //lcd.print(str);

  sensorValue = analogRead(analogInPin);
  // map it to the range of the analog out:
  FM_Freq = mymap(sensorValue, 0.00, 1023.00, 88.0, 108.00 );
  // change the analog out value:

  // print the results to the Serial Monitor:
  Serial.print("sensor = ");
  Serial.print(sensorValue);
  Serial.print("\t output = ");
  Serial.println(FM_Freq);
  str = FM_Freq;
  lcd.print(str);
  radio.setFrequency(FM_Freq);
  delay(3000);
  // 100.1 Amrutha varshini and Amrutha Mahal
  //for ( float i = 99; i <102; i = i + 0.1 ) {
      FM_Freq=94.3;
      str = FM_Freq;
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print(str);
      Serial.println ( str);
      radio.setFrequency(FM_Freq);
      delay(3000);
  //}
  while(1);
  /*
  FM_Freq=98.5;  
  str = FM_Freq;
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print(str);

  radio.setFrequency(FM_Freq);
  delay(3000);
  
  FM_Freq=104.00;  
  str = FM_Freq;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("WH Inventors Stn");
  lcd.setCursor(0,1);
  lcd.print(str);

  radio.setFrequency(FM_Freq);
  delay(3000);
  */
}
