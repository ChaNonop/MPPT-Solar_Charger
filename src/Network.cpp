#include "Network.h"
#include <Arduino.h>

static Network* _net_instance = nullptr;

void mqttCallback(char* topic, uint8_t* payload, unsigned int length) {
  if (_net_instance) _net_instance->handleCallback(topic, payload, length);
}

Network::Network() : _client(_espClient) {
  _net_instance = this;
  _client.setServer(MQTT_SERVER, MQTT_PORT);
  _client.setCallback(mqttCallback);
}

void Network::conncetWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting WiFi");
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (millis() - start > 15000) break; // timeout
  }
  Serial.println();
  Serial.print("WiFi Status: ");
  Serial.println(WiFi.status());
}

void Network::connectMQTT() {
  if (!_client.connected()) {
    reconnectMQTT();
  }
}

void Network::reconnectMQTT() {
  while (!_client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (_client.connect(MQTT_CLIENT, MQTT_USER, MQTT_PASS)) {
      Serial.println("connected");
      _client.subscribe("/Command");
    } else {
      Serial.print("failed, rc=");
      Serial.print(_client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void Network::loop_connect_MQTT() {
  if (!_client.connected()) {
    reconnectMQTT();
  }
  _client.loop();
}

void Network::Publish_Sensor(uint8_t resistor, float voltage, bool buttonState) {
  char payload[128];
  int len = snprintf(payload, sizeof(payload), "{\"res\":%u,\"v\":%.3f,\"btn\":%u}", resistor, voltage, buttonState ? 1 : 0);
  _client.publish("/sensor", payload, (unsigned int)len);
}

void Network::handleCallback(char* topic, uint8_t* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);
  if (String(topic) == "/Command") {
    if (message == "ON" || message == "1") {
      Serial.println("Command: ON");
    } else if (message == "OFF" || message == "0") {
      Serial.println("Command: OFF");
    }
  }
}
