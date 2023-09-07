#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <LiquidCrystal_I2C.h>
#include <DHTesp.h>
#include <Wire.h>
#include <NTPClient.h>


// ZUGANGSDATEN ssid/password
#include <E:\arduino-1.8.19-portable\projects\tmphum\esp8266wificredt.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);
WiFiServer server(80);
DHTesp dht;
const long utcOffsetInSeconds = 7200;
char daysOfTheWeek[7][12] = {"So", "Mo", "Die", "Mi", "Do", "Fr", "Sa"};
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

int BUILDIN_LED = 2;

void ntpTimer(){
  timeClient.update();

  lcd.setCursor(3,0);
  lcd.print(daysOfTheWeek[timeClient.getDay()]);
  lcd.print(", ");
  lcd.print(timeClient.getHours());
  lcd.print(":");
  lcd.print(timeClient.getMinutes());
  lcd.print(":");
  lcd.println(timeClient.getSeconds());
  // lcd.print(timeClient.getFormattedTime());
}

void setupWiFi(){
  lcd.clear();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Verbindet mit SSID");
  lcd.setCursor(9,1);
  lcd.print(ssid);

  // WiFi.begin(ssid, password); 
  int index = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    index = index + 1;
    Serial.print(index,18);
    Serial.print("-");
    digitalWrite(BUILDIN_LED, LOW);
    delay(500);
    digitalWrite(BUILDIN_LED, HIGH);
    delay(500);
  }
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Server gestartet");
  server.begin(); 
  lcd.setCursor(0,1);
  lcd.print("IP-Adresse");
  lcd.setCursor(0,2);
  lcd.print(WiFi.localIP().toString());
  delay(10000);
  lcd.clear();
}

void writeResponse(WiFiClient client){
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>"); 
  client.println("<head>"); 
  client.println("<meta http-equiv='refresh' content='1; URL=http://"+WiFi.localIP().toString()+"'/>"); 
  client.println("</head>");   
  client.println("<body>"); 
  
  float hum = dht.getHumidity();
  float tmp = dht.getTemperature();
  
  if (isnan(hum) || isnan(tmp)) {
    client.println("Fehler beim lesen der Sensorwerte!");
  } else {
    client.print("Temperatur: ");
    client.print(String(tmp,2));
    client.println(" &deg;C");

    client.println("<br/>");

    client.print("rel. Luftfeuchtigkeit: ");
    client.print(String(hum,2));
    client.println(" %");
  }
  client.println("</body>"); 
  client.println("</html>"); 
}

void localResponse(){
  float hum = dht.getHumidity();
  float tmp = dht.getTemperature();
  
  if (isnan(hum)||isnan(tmp)){
    Serial.print("Keine Sensorwerte");
  } else {
    lcd.setCursor(0,2);
    lcd.print("Temp: ");
    lcd.setCursor(6,2);
    lcd.print(tmp);
    lcd.print(" C");

    lcd.setCursor(0,3);
    lcd.print("Hum: ");
    lcd.setCursor(5,3);
    lcd.print(hum);
    lcd.print(" %");
    }
}

void setup(){
  Serial.begin(115200);
  WiFi.begin(ssid, password); 
  timeClient.begin();
  dht.setup(14, DHTesp::DHT22);
  pinMode(BUILDIN_LED, OUTPUT);
}

void loop(){
  
  if(WiFi.status() != WL_CONNECTED){
    setupWiFi();
  } else {
      digitalWrite(BUILDIN_LED, LOW);
      localResponse();
      ntpTimer();
    }

  WiFiClient client = server.accept();
   if (!client) {
     return;
   }

   Serial.println("Neuer Client verbunden.");
   while(!client.available()){
     delay(1);
   }

   writeResponse(client);
   delay(1000);
}
