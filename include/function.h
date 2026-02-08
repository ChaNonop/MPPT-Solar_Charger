#ifndef FUNCTION_H
#define FUNCTION_H

#include <Arduino.h>
#include "Pin.h"

class Button{
private:
  Config _config;
  volatile bool _pressed;
  volatile unsigned long _buttonState;; //เวลาที่เกิด interrupt
  volatile unsigned long _lastInterruptTime;

  // Callback function pointer 
  void (*_Butonn_callback)();

  // Mutex สำหรับ thread safety
  #ifdef ESP32
  portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
  #endif

public:
  Button(const Config& config);
  // เริ่มต้นการทำงาน - แนบ ISR
  void button_begin(void (*button_isr)());
  void setCallback(void (*callback)());
  #ifdef ESP32
  void IRAM_ATTR handleInterrupt();
  #else
  void ICACHE_RAM_ATTR handleInterrupt();
  #endif
  void update();

  const char*getName() const; // ดึงชื่อปุ่ม
  uint8_t getPin() const;    // ดึงหมายเลข pin
  bool isPressed() ;   // เช็คว่าปุ่มถูกกดหรือไม่
}; 

// === Class Snesor ===
class Sensor {
private:
  uint8_t _rPin;

public:
  Sensor();
  void begin();
  // bool readbutton();

  uint8_t read_R();
  float Convert_voltage();
  uint8_t read_Current();
  uint8_t read_temp();
};

class Led_state {
public:
  Led_state(uint8_t ledPin);
  void begin();
  void set(bool on);
  void toggle();
private:
  uint8_t _ledPin;
  bool _ledState;
};

#endif 