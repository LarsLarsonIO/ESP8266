#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>

#include <E:\arduino-1.8.19-portable\projects\tmphum\esp8266wificredt.h>

void setup() {
    WiFi.begin(ssid, password);
    ArduinoOTA.setHostname("router");
    ArduinoOTA.setPassword("2133");
    ArduinoOTA.begin();
}

void loop(){
    ArduinoOTA.handle();
}
