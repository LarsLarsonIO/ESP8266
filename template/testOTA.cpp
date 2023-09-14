#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_SSID_PASSWORD";

void setup() {
    WiFi.begin(ssid, password);
    ArduinoOTA.setHostname("");
    ArduinoOTA.setPassword("");
    ArduinoOTA.begin();
}

void loop(){
    ArduinoOTA.handle();
}
