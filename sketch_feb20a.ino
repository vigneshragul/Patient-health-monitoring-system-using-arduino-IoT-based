#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
char auth[] = "b00152a23aff4e2faa59198cc4c3ad94";
LiquidCrystal_I2C lcd(0x27, 16, 2); // Address for LCD | Columns and Rows
int myBPM;
int outputpin = 0;  //Pin allocation for temperature sensor
char ssid[] = "Amrutha";  // Username
char pass[] = "ammu1996";  // Password
WiFiClient client;
unsigned long myChannelNumber = 437468;  // Thingspeak channel ID
const char * myWriteAPIKey = "LA9JJV6S4YPMJX0A";  // Thingspeak Write API Key

void setup()
{
  Serial.begin(9600);  // Baud Rate
  WiFi.begin(ssid, pass);
  ThingSpeak.begin(client);
  Blynk.begin(auth, ssid, pass);
  Wire.begin();
  lcd.init();
  lcd.backlight();
}

void loop()
{
  Wire.requestFrom(0x07, 6);  // Address bus
  myBPM = Wire.read();
  Serial.print("BPM: ");
  Serial.println(myBPM);
  Blynk.run();
  int rawvoltage = analogRead(outputpin);
  float millivolts = (rawvoltage / 1024.0) * 3300;
  float celsius = millivolts / 10;
  Serial.print("Temp: ");
  Serial.print(celsius);
  Serial.println(" *C");
  //Serial.print((celsius * 9) / 5 + 32);
  //Serial.println(" degrees Fahrenheit");
  lcd.setCursor(0, 0);
  lcd.print("BPM: ");
  lcd.print(myBPM);
  lcd.print(" ");
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(celsius);
  lcd.print(" *C");
  lcd.print(" ");
  if (myBPM > 120)
  {
    Blynk.notify("Person is with high BPM");
  }
  else if(myBPM<70){
    Blynk.notify("Person is with low BPM");
  }
  else{
    Blynk.notify("Person is with normal BPM");
  }
  ThingSpeak.writeField(myChannelNumber, 1, myBPM, myWriteAPIKey);  // Sending data to field 1
  delay(1000);
  ThingSpeak.writeField(myChannelNumber, 2, celsius, myWriteAPIKey);  // Sending data to field 2
  delay(1000);
}
