#ifndef FUNCTION_H
#define FUNCTION_H

#include <Arduino.h>
#include "Pin.h"

Class Button{
private:
  Config _config;
  volatile bool _pressed;
  volatile unsigned long _lastInterruptTime; //เวลาที่เกิด interrupt

  // Callback function pointer 
  void (*Butonn_callback)();

  // Mutex สำหรับ thread safety
  portMUX_TYPE mux;
public:
  Button(const Config& _config);
  // เริ่มต้นการทำงาน - แนบ ISR
  void button_begin(void (*button_isr)());
  void setCallback(void (*callback)());
  voide IRAM_ATTR handleInterrupt();
  void update();

  cosnt char*getName() const; // ดึงชื่อปุ่ม
  uint8_t getPin() const;    // ดึงหมายเลข pin
  bool isPressed() const;   // เช็คว่าปุ่มถูกกดหรือไม่
}; 

class Sensor {
private:
  uint8_t _rPin;

public:
  Sensor(){
    read_R();
    Convert_voltage();
    read_Current();
    read_temp();
  };

  Sensor(uint8_t read_R,uint8_t rPin,
    float Convert_voltage,uint8_t read_Current,uint8_t read_temp);
  
  void begin();
  // bool interrupt_button_set();
  // bool readbutton();

  uint8_t read_R();
  float Convert_voltage();
  uint8_t read_Current();
  uint8_t read_temp();
};

// class Led_state {
// public:
//   Led_state(uint8_t ledPin);
//   void begin();
//   void set(bool on);
// private:
//   uint8_t _ledPin;
// };

#endif 