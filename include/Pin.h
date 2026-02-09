#ifndef PIN_H
#define PIN_H

#include <Arduino.h>

  // --- Config สำหรับ ESP32 ---
  #define VOLTAGE_SOLAR_PIN 34
  #define VOLTAGE_BATTERY_PIN 35
  #define CURRENT_PIN 32
  #define TEMP_PIN 33



  // แรงดันอ้างอิง
  // Arduino Uno/Nano: 5.0V
  // ESP32/ESP8266: 3.3V
  #define VOLTAGE_REFERENCE 3.3
  #define ADC_VALUE 4095 

  // ค่า Alpha สำหรับ EMA Filter
  // ค่าน้อย (0.1) = Smooth มาก แต่ตอบสนองช้า
  // ค่ามาก (0.5) = ตอบสนองเร็ว แต่ Smooth น้อย
  // แนะนำ: 0.2 สำหรับการใช้งานทั่วไป
  // #define EMA_FILTER_ALPHA  0.2

  // sampling เฉลี่ยค่า
  #define SAMPLE_COUNT  10

  // const int Pin_sensor[Num_Pin] = [34,35,32,33];
  // const uint8_t Num_Pin = 4;
  // const int Num_Sample = 10;
  // // เก็บค่าอ่าน
  // int sensorValues[Num_Pin];
  // const uint8_t sample = 10;
  // const uint8_t Voltage_Pin_Solar = 34;
  // const uint8_t Voltage_Pin_Battery = 35;
  // const uint8_t Current_Pin = 32;
  // const uint8_t Temp_Pin =33;

  const uint8_t led_Pin = 2;

    struct Config {
    uint8_t Button_Pin;
    const char* Button_Name;
    unsigned long debounceDelay;
    int button_edge;
  };
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
