#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>

#include <C:\Users\Crypdolars\Desktop\git\ESP8266\template\wifi_settings.cpp>

void setup() {
    WiFi.begin(ssid, password);
    ArduinoOTA.setHostname("");
    ArduinoOTA.setPassword("");
    ArduinoOTA.begin();
}

void loop(){
    ArduinoOTA.handle();
}
