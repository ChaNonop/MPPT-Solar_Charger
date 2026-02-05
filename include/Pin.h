#ifndef PIN_H
#define PIN_H

#include <Arduino.h>

// --- Config สำหรับ ESP32 ---
#ifdef ESP32
  const uint8_t R_PIN = 34;        // Analog input ESP32
  const uint8_t button_Pin = 0;   
  const uint8_t led_Pin = 2;     
#elif defined(ESP8266)
  const uint8_t R_PIN = A0;       
  const uint8_t button_Pin = D3;  
  const uint8_t led_Pin = LED_BUILTIN;
#else
  const uint8_t R_PIN = A0;
  const uint8_t button_Pin = 0;
  const uint8_t led_Pin = LED_BUILTIN;
#endif

// --- Config สำหรับ ESP8266 ---
#ifdef ESP8266
    #define SENSOR_TEMP_PIN     A0  
    #define LED_STATUS_PIN      D4  
#endif

#endif 

