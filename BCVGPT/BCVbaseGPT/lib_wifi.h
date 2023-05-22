//lib_wifi.h
#ifndef LIB_WIFI_H
#define LIB_WIFI_H

#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <Ticker.h>

const uint8_t pinLedWifi = D4;
ESP8266WebServer server(80);

Ticker ticker;

void parpadeoLedWifi() {
  static bool estado = false;
  digitalWrite(pinLedWifi, estado ? LOW : HIGH);
  estado = !estado;
}

void conectarWiFi() {
  pinMode(pinLedWifi, OUTPUT);
  digitalWrite(pinLedWifi, LOW);

  ticker.attach(0.2, parpadeoLedWifi);

  Serial.begin(115200);
  delay(500);

  WiFiManager wifiManager;
  if (!wifiManager.autoConnect("Matrix LED")) {
    Serial.println("Failed to connect to WiFi, starting AP mode");
    WiFi.mode(WIFI_AP);
    WiFi.softAP("Matrix LED");
  }

  ticker.detach();
  digitalWrite(pinLedWifi, HIGH);
}

#endif