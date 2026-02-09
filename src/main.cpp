#include <Arduino.h>
#include "function.h"
#include "Network.h"
#include "secret.h"
#include "Pin.h"
#include <WiFi.h>
   
#define DEVICE_ID "ESP32_Board"

Sensor sensor;
Network network;
Led_state led(led_Pin);


//===== สร้าง BUTTON OBJECTS =====
// สร้าง object จาก config
Button buttonUp(PinConfig::ButtonUp);
Button buttonDown(PinConfig::ButtonDown);
Button buttonLeft(PinConfig::Buttonleft);
Button buttonRight(PinConfig::ButtonRight);

unsigned long lastMsg = 0;
const long interval = 10000; // 10 วินาที

// --- ISR Wrapper Functions ---
// แยกฟังก์ชัน ISR ของแต่ละปุ่ม 
void IRAM_ATTR ISR_Up() { buttonUp.handleInterrupt();}
void IRAM_ATTR ISR_Down() { buttonDown.handleInterrupt();}
void IRAM_ATTR ISR_Left() { buttonLeft.handleInterrupt();}
void IRAM_ATTR ISR_Right() { buttonRight.handleInterrupt();}


void setup() {
  Serial.begin(115200);
  //   while (!Serial){
  //   delay(10);
  // }
  delay(100);

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
  network.connectWifi(); 
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

  sensor.update();


  unsigned long now = millis();
    if (now - lastMsg > interval) { 
        lastMsg = now;

        // ดึงข้อมูลจาก Sensor Class
        SensorData data = sensor.getData();
        
        // Debug ใน Serial
        sensor.Print_sensor();
        bool btn_state = buttonUp.isPressed();

        network.Publish_Sensor(data.Voltage_solar,data.Voltage_battery, data.Current, data.Temp,(int)data.Power, btn_state);
        led.toggle();
      }
}

  