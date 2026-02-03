#ifndef PIN_H
#define PIN_H

#include <Arduino.h>

// --- Config สำหรับ ESP32 ---
#ifdef ESP32
  const uint8_t R_PIN = 34;        // Analog input ESP32
  const uint8_t button_Pin = 0;   // example: GPIO0
  const uint8_t led_Pin = 2;      // Built-in LED (GPIO2)
#elif defined(ESP8266)
  const uint8_t R_PIN = A0;       // ESP8266 analog
  const uint8_t button_Pin = D3;  // NodeMCU D3 (GPIO0) - adjust if needed
  const uint8_t led_Pin = LED_BUILTIN;
#else
  const uint8_t R_PIN = A0;
  const uint8_t button_Pin = 0;
  const uint8_t led_Pin = LED_BUILTIN;
#endif

// --- Config สำหรับ ESP8266 ---
#ifdef ESP8266
    #define SENSOR_TEMP_PIN     A0  // ESP8266 มี Analog ขาเดียว
    #define LED_STATUS_PIN      D4  // ขา Built-in LED
#endif

#endif //Pin.h

// // --- Config สำหรับ ESP32 ---
// #ifndef ESP32
//     #define R_Pin = D34;
//     #define button_Pin = D19;
//     #define LED_PIN = D13;
// #endif


// // --- Config สำหรับ ESP8266 ---
// #ifdef ESP8266
//     #define SENSOR_TEMP_PIN      A0  // ESP8266 มี Analog ขาเดียว
//     #define LED_STATUS_PIN       D4  // ขา Built-in LED
// #endif


