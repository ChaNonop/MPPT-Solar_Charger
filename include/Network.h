#ifndef NETWORK_H
#define NETWORK_H

#include <Arduino.h>
#ifdef ESP32
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <PubSubClient.h>
#include "config.h"

class Network {
public:
  Network();
  void conncetWifi();           // intentionally keep typo to match main.cpp usage
  void connectMQTT();
  void loop_connect_MQTT();
  void Publish_Sensor(uint8_t resistor, float voltage, bool buttonState);
  void handleCallback(char* topic, uint8_t* payload, unsigned int length); // made public

private:
  WiFiClient _espClient;
  PubSubClient _client;
  void reconnectMQTT();
};

#endif // NETWORK_H