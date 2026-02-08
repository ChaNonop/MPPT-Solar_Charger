#include <Arduino.h>
#include "Sensor_function.h"
#include "Network.h"
#include "config.h"
#include "Pin.h"

#ifdef ESP32
  #include <WiFi.h>
  #define POT_PIN 34      
  #define DEVICE_ID "ESP32_Board"

#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
  #define POT_PIN A0       
  #define DEVICE_ID "ESP8266_Board"
#endif

// สร้าง Objects
Sensor sensor(R_PIN, button_Pin);
Network network;
Led_state led(led_Pin);

unsigned long lastMsg = 0;
const long interval = 10000; // 10 วินาที

void setup() {
  Serial.begin(115200);

  sensor.begin();
  led.begin();
  
  while (!Serial){
    delay(10);
  }
  WiFi.mode(WIFI_STA);

  network.conncetWifi(); 
  network.connectMQTT();
}

void loop(){
  // ให้ MQTT ทำงานตลอดเวลา
  network.loop_connect_MQTT();

  long now = millis();
    if (now - lastMsg > interval) { //จับเวลาส่งข้อมูลทุก ๆ 10 วินาที
        lastMsg = now;

        uint8_t Resistor = sensor.read_R();
        float voltage = sensor.Convert_voltage();
        bool btn_state = sensor.readbutton(); 
        network.Publish_Sensor(Resistor,voltage,btn_state);
        }
  }


// void callback(char* topic, byte* payload, unsigned int length) {
//   Serial.print("Message arrived [");
//   Serial.print(topic);
//   Serial.print("] ");

//   String message;
//   for (unsigned int i = 0; i < length; i++) {
//     message = message + char(payload[i]);
//   }
//   Serial.println(message);

//   // Subscribe /Command
//   if(String(topic) == "/Command") {
//     // รองรับทั้ง "ON" และ "1"
//     if (message == "ON" || message == "1"){
//       digitalWrite(LED_1, HIGH);
//       Serial.println("LED ON"); 
//     }
//     else if (message == "OFF" || message == "0") {
//       digitalWrite(LED_1, LOW);   
//       Serial.println("LED OFF"); 
//     }
//   } 
// }
