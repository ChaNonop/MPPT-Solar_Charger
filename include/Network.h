#ifndef NETWORK_H
#define NETWORK_H

#include <Arduino.h>

#ifdef ESP32
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif

#include <PubSubClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// #include <U8g2lib.h>

#include "config.h"

class Network {
private:
  WiFiClient _espClient;
  PubSubClient _client;
  WiFiUDP _ntp;
  NTPClient* _timeClient; // ใช้ Pointer เพื่อจัดการ Object
  unsigned long epochTime;
  void reconnectMQTT();

public:
  Network();
  void ntp_setup();
  void conncetWifi();           // intentionally keep typo to match main.cpp usage
  void connectMQTT();
  void loop_connect_MQTT();
  void Publish_Sensor(uint8_t resistor, float voltage, bool buttonState);
  void Callback(char* topic, uint8_t* payload, unsigned int length); // made public
};

#endif 