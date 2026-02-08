#ifndef PIN_H
#define PIN_H

#include <Arduino.h>

// --- Config สำหรับ ESP32 ---
#ifdef ESP32
  const uint8_t R_PIN = 34;   // Analog input ESP32  
  const uint8_t Voltage_Pin;  //
  const uint8_t Current_Pin;
  const uint8_t Temp_Pin;

  const uint8_t led_Pin = 2; 
  struct Config {
    uint8_t Button_Pin;
    const char* Button_Name;
    unsigned long debounceDelay;
    int button_state;
  };
  namespace PinConfig {
    const Config ButtonUp ={
      .Button_Pin = 18, //ใช้ GPIO 18 
      .Button_Name = "Button Up", //ตั้งชื่อปุ่ม
      .debounceDelay = 50, // debounce 50 ms
      .button_state = FALLING // ตรวขอบขาลง
    };
  const Config ButtonDown ={
      .Button_Pin = 17, //ใช้ GPIO 17
      .Button_Name = "Button Down", //ตั้งชื่อปุ่ม
      .debounceDelay = 50,
      .button_state = FALLING // ตรวขอบขาลง    
  };
  const Config Buttonleft ={
    .Button_Pin = 16, //ใช้ GPIO 17
    .Button_Name = "Button left", //ตั้งชื่อปุ่ม
    .debounceDelay = 50, 
    .button_state = FALLING // ตรวขอบขาลง  
  };
    const Config ButtonRight ={
    .Button_Pin = 16, //ใช้ GPIO 17
    .Button_Name = "Button Right", //ตั้งชื่อปุ่ม
    .debounceDelay = 50, 
    .button_state = FALLING // ตรวขอบขาลง  
  };
}
#endif

// #elif defined(ESP8266)
//   const uint8_t R_PIN = A0;       
//   const uint8_t button_Pin = D3;  
//   const uint8_t led_Pin = LED_BUILTIN;
// #else
//   const uint8_t R_PIN = A0;
//   const uint8_t button_Pin = 0;
//   const uint8_t led_Pin = LED_BUILTIN;
#endif

// --- Config สำหรับ ESP8266 ---
#ifdef ESP8266
    #define SENSOR_TEMP_PIN     A0  
    #define LED_STATUS_PIN      D4  
#endif

