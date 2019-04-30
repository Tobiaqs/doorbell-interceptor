#include <ESP8266WiFi.h>
#include "secrets.h"
#include <WiFiUdp.h>

WiFiUDP Udp;

void setup() {
  pinMode(D1, OUTPUT);
  pinMode(D2, INPUT_PULLUP);
  digitalWrite(D1, LOW);
  
  WiFi.setAutoReconnect(true);
  WiFi.begin(wifiSsid, wifiPassword);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

unsigned long lastRing = 0;
bool prevValue = true;

void loop() {
  bool value = digitalRead(D2);
  digitalWrite(D1, !value);

  if (!value && prevValue && (millis() - lastRing > debounce || millis() < lastRing)) {
    if (WiFi.status() == WL_CONNECTED) {
      lastRing = millis();
      Udp.beginPacket(receiverIp, receiverPort);
      Udp.write(receiverSecret);
      Udp.endPacket();
    }
  }

  prevValue = value;
}
