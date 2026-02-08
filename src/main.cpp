#include <Arduino.h>
#include "function.h"
#include "Network.h"
#include "secret.h"
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
Sensor sensor;
Network network;
Led_state led(led_Pin);


//===== สร้าง BUTTON OBJECTS =====
// สร้าง object จาก config ที่กำหนดไว้ใน PinConfig.h
Button buttonUp(PinConfig::ButtonUp);
Button buttonDown(PinConfig::ButtonDown);
Button buttonLeft(PinConfig::Buttonleft);
Button buttonRight(PinConfig::ButtonRight);

unsigned long lastMsg = 0;
const long interval = 10000; // 10 วินาที

// --- ISR Wrapper Functions ---
// เราต้องแยกฟังก์ชัน ISR ของแต่ละปุ่ม เพื่อให้รู้ว่าปุ่มไหนถูกกด
// เพราะ attachInterrupt ต้องการฟังก์ชัน void func() ที่ไม่มี parameter
void IRAM_ATTR ISR_Up() {
  buttonUp.handleInterrupt();
}
void IRAM_ATTR ISR_Down() {
  buttonDown.handleInterrupt();
}
void IRAM_ATTR ISR_Left() {
  buttonLeft.handleInterrupt();
}
void IRAM_ATTR ISR_Right() {
  buttonRight.handleInterrupt();
}


void setup() {
  Serial.begin(115200);
    while (!Serial){
    delay(10);
  }
  // 1. Setup Buttons & Attach ISRs
  buttonUp.button_begin(ISR_Up);
  buttonDown.button_begin(ISR_Down);
  buttonLeft.button_begin(ISR_Left);
  buttonRight.button_begin(ISR_Right);

  // 3. setup Component
  sensor.begin();
  // led.begin();

  //4. setup Network
  WiFi.mode(WIFI_STA);
  network.conncetWifi(); 
  network.connectMQTT();
  network.ntp_setup();
}

void loop(){
  // 1. ให้ MQTT ทำงานตลอดเวลา
  network.loop_connect_MQTT();

  // 2. Update Buttons (เช็คสถานะจาก ISR ใน main loop)
  buttonUp.update();
  buttonDown.update();
  buttonLeft.update();
  buttonRight.update();

  unsigned long now = millis();
    if (now - lastMsg > interval) { //จับเวลาส่งข้อมูลทุก ๆ 10 วินาที
        lastMsg = now;

        uint8_t Resistor = sensor.read_R();
        float voltage = sensor.Convert_voltage();

        // ตัวอย่าง: เช็คสถานะปุ่มใดปุ่มหนึ่งเพื่อส่งขึ้น MQTT (หรือจะส่ง flag อื่นๆ)
        bool btn_state = buttonUp.isPressed();

        network.Publish_Sensor(Resistor,voltage,btn_state);
        led.toggle();
      }
}

  