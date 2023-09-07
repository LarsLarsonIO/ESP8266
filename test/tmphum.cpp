
/*
  Download Treiber: https://cdn.shopify.com/s/files/1/1509/1638/files/ch340.zip?v=1683899825

  Komponenten:
    D1 mini ESP8266 12F
    LCD 2004 I2C Display
    DHT22 Sensor
  
  Bibliotheken:
    ESP8266WiFi
    LiquidCrystal_I2C
    DHTesp
    Wire
    time
  
  Zugang zum Projekt im Browser
*/

#include <E:\arduino-1.8.19-portable\projects\tmphum\esp8266wificredt.h>

#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <DHTesp.h>
#include <Wire.h>
#include <time.h>

WiFiServer server(80);
LiquidCrystal_I2C lcd(0x27, 20, 4);
DHTesp dht;

int BUILDIN_LED = 2;

String img1 = "C:/Users/Crypdolars/Desktop/backup_esp8266/esp8266_12f_home/icons/tem2.png";
String img2 = "C:/Users/Crypdolars/Desktop/backup_esp8266/esp8266_12f_home/icons/hum3.png";

void writeResponse(WiFiClient client){
  float hum = dht.getHumidity();
  float tmp = dht.getTemperature();

  client.println("<!DOCTYPE HTML><html>");
  client.println("<head>");
  client.println("<meta name='viewport' http-equiv='refresh' content='5; URL=http://" + WiFi.localIP().toString() + "'/>");
  client.println("<style>");
  client.println("html {");
  client.println("font-family: Arial;");
  client.println("display: inline-block;");
  client.println("margin: 0px auto;");
  client.println("text-align: center;");
  client.println("}");
  client.println("h2 { font-size: 3.0rem; }");
  client.println("p { font-size: 3.0rem; }");
  client.println(".units { font-size: 1.2rem; }");
  client.println(".dht-labels{");
  client.println("font-size: 1.5rem;");
  client.println("vertical-align:middle;");
  client.println("padding-bottom: 15px;");
  client.println("}");
  client.println("</style>");
  client.println("</head>");
  client.println("<body>");
  client.println("<h2>D1 mini ESP8266 12F</h2>");
  client.println("<p>");
  client.println("<img src=" + img1 + ">");
  client.println("<span class='dht-labels'>Temperature</span>");
  client.println("<span id='temperature'>" + String(tmp,2) + "</span>");
  client.println("<sup class='units'>&deg;C</sup>");
  client.println("</p>");
  client.println("<p>");
  client.println("<img src=" + img2 + ">");
  client.println("<span class='dht-labels'>Humidity</span>");
  client.println("<span id='humidity'>" + String(hum,2) + "</span>");
  client.println("<sup class='units'>%</sup>");
  client.println("</p>");
  client.println("</body>");
  client.println("</html>");   
}

void setup(){
  Serial.begin(115200);
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("D1 ESP8266 12F");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
    lcd.setCursor(0,1);
    lcd.print("OFFLINE");
    }

  Serial.println("Server gestartet");
  Serial.println("IP-Adresse");
  Serial.println(WiFi.localIP().toString());
  server.begin();

  dht.setup(14, DHTesp::DHT22); // GPIO 14 == D5
  
  pinMode(BUILDIN_LED, OUTPUT);
}

void loop(){
  float hum = dht.getHumidity();
  float tmp = dht.getTemperature();

  if (isnan(hum) || isnan(tmp)){
    Serial.println("> Failed to read from DHT Sensor <");
    return;
  } else {
      // Zeile 3 LCD
      lcd.setCursor(0, 2);
      lcd.print("Hum: ");
      lcd.print(hum);
      lcd.print(" % ");  
      // Zeile 4 LCD
      lcd.setCursor(0, 3);
      lcd.print("Temp: ");
      lcd.print(tmp);
      lcd.print("C");
  
      digitalWrite(BUILDIN_LED, LOW);
      delay(500);
      digitalWrite(BUILDIN_LED, HIGH);
      delay(500);
      }

  if(WiFi.status() != WL_CONNECTED){
    setup();
  }

  WiFiClient client = server.accept();
  if (!client){
    return;
  }

  while(!client.available()){
   delay(1);
  }

  Serial.println("Client verbunden");
  lcd.setCursor(0,1);
  lcd.print("On @ " + WiFi.localIP().toString());

  writeResponse(client);
  delay(1500);
}