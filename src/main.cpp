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


// const char* ssid = "Somwang";
// const char* password = "03143681"; 
// const char* mqtt_server = "broker.hivemq.com"; 
// const int mqtt_port = 1883;

// // Client ID ควรจะไม่ซ้ำใคร
// const char* mqtt_Client = "ESP8266_My"; 
// const char* mqtt_username = "";  
// const char* mqtt_password = ""; 

// WiFiClient espClient; 
// PubSubClient client(espClient);


// void reconnect() {
//   while (!client.connected()) {
//     Serial.print("Attempting MQTT connection...");
//     if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) { 
//       Serial.println("connected");
      
//       client.subscribe("/Command"); 
//       Serial.println("Subscribed to /Command");
//     }
//     else {
//       Serial.print("failed, rc=");
//       Serial.print(client.state());
//       Serial.println(" try again in 5 seconds");
//       delay(5000);
//     }
//   }
// }

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

// void loop() {
//   int currentButtonState = digitalRead(Pin_button);

//   // Logic ป้องกันการส่งรัวๆ
//   // ตรวจสอบว่าค่าปุ่มเปลี่ยนไปจากรอบที่แล้วหรือไม่
//   if (currentButtonState != lastButtonState) {
    
//     // ถ้าสถานะปัจจุบันคือ LOW (แปลว่าเพิ่งถูกกด)
//     if (currentButtonState == LOW) {
//       // 1. สั่ง LED Built-in ติด (Active LOW)
//       digitalWrite(LED_BUILTIN, LOW); 
      
//       // 2. ส่ง Publish ค่า 1 ไปที่ /non
//       Serial.println("Button Pressed -> Publishing '1'");
//       client.publish("/non","1"); 
//     } 
//     // ถ้าสถานะปัจจุบันคือ HIGH (แปลว่าเพิ่งปล่อยมือ)
//     else {
//       // สั่ง LED Built-in ดับ
//       digitalWrite(LED_BUILTIN, HIGH); 
//     }
    
//     // หน่วงเวลาเล็กน้อยแก้สั่น (Debounce)
//     delay(50); 
//   }
  
//   // จำค่าสถานะไว้เทียบในรอบถัดไป
//   lastButtonState = currentButtonState;
// }