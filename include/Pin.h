#ifndef PIN_H
#define PIN_H

#include <Arduino.h>
  struct Config {
    uint8_t Button_Pin;
    const char* Button_Name;
    unsigned long debounceDelay;
    int button_edge;
  };

// --- Config สำหรับ ESP32 ---
#ifdef ESP32
  const uint8_t R_PIN = 34;   // Analog input ESP32  
  const uint8_t Voltage_Pin = 34;  //
  const uint8_t Current_Pin = 32;
  const uint8_t Temp_Pin =33;

  const uint8_t led_Pin = 2; 
  namespace PinConfig {
    const Config ButtonUp ={
      .Button_Pin = 18, //ใช้ GPIO 18 
      .Button_Name = "Button Up", //ตั้งชื่อปุ่ม
      .debounceDelay = 50, // debounce 50 ms
      .button_edge = FALLING // ตรวขอบขาลง
    };
  const Config ButtonDown ={
      .Button_Pin = 17, //ใช้ GPIO 17
      .Button_Name = "Button Down", //ตั้งชื่อปุ่ม
      .debounceDelay = 50,
      .button_edge = FALLING // ตรวขอบขาลง    
  };
  const Config Buttonleft ={
    .Button_Pin = 16, //ใช้ GPIO 16
    .Button_Name = "Button Left", //ตั้งชื่อปุ่ม
    .debounceDelay = 50, 
    .button_edge = FALLING // ตรวขอบขาลง  
  };
    const Config ButtonRight ={
    .Button_Pin = 15, //ใช้ GPIO 15
    .Button_Name = "Button Right", //ตั้งชื่อปุ่ม
    .debounceDelay = 50, 
    .button_edge = FALLING // ตรวขอบขาลง  
  };
}
#endif

// --- Config สำหรับ ESP8266 ---
#ifdef ESP8266
    #define SENSOR_TEMP_PIN     A0  
    #define LED_STATUS_PIN      D4  
#endif

#endif

