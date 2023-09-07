#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <LiquidCrystal_I2C.h>

#include <E:\arduino-1.8.19-portable\projects\tmphum\esp8266wificredt.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

const long utcOffsetInSeconds = 3600;

char daysOfTheWeek[7][12] = {"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup(){
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    lcd.print ( "." );
  }

  timeClient.begin();
}

void loop() {
  timeClient.update();

  lcd.print(daysOfTheWeek[timeClient.getDay()]);
  lcd.print(", ");
  lcd.print(timeClient.getHours());
  lcd.print(":");
  lcd.print(timeClient.getMinutes());
  lcd.print(":");
  lcd.print(timeClient.getSeconds());
  //lcd.println(timeClient.getFormattedTime());

  delay(1000);
}