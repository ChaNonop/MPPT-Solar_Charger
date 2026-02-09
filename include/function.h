#ifndef FUNCTION_H
#define FUNCTION_H

#include <Arduino.h>
#include "Pin.h"

#define NUM_SENSOR 4

struct SensorData { //ค่าที่จะส่งผ่าน broker
  // int rawValue;
  float Voltage_solar;
  float Voltage_battery;
  float Current;
  float Temp;
  float Power;
  int dutyCycle;
};

class Button{
private:
  Config _config;
  volatile unsigned long _buttonState;; //เวลาที่เกิด interrupt
  volatile unsigned long _lastInterruptTime;

  // Callback function pointer 
  void (*_Butonn_callback)();

  // Mutex สำหรับ thread safety
  portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

public:
  Button(const Config& config);
  // เริ่มต้นการทำงาน - แนบ ISR
  void button_begin(void (*button_isr)());
  void setCallback(void (*callback)());
  void IRAM_ATTR handleInterrupt();
  void update();

  const char*getName() const; // ดึงชื่อปุ่ม
  uint8_t getPin() const;    // ดึงหมายเลข pin
  bool isPressed() ;   // เช็คว่าปุ่มถูกกดหรือไม่
}; 

// === Class Sensor ===
class Sensor {
private:
  SensorData _currentData;
  const uint8_t sensorPins[NUM_SENSOR] = {VOLTAGE_SOLAR_PIN, VOLTAGE_BATTERY_PIN, CURRENT_PIN, TEMP_PIN};

  // Helper functions (Private)
  int readAverage(uint8_t pin, uint8_t count);
  float rawToVoltage(int raw, float multiplier = 1.0); // Multiplier คืออัตราทด Voltage Divider
  float rawToCurrent(int raw);
  float rawToTemp(int raw);

public:
  Sensor();
  void begin();
  void update();
  SensorData getData() const;

  void Print_sensor();
  void Display_sensor();
};
class Algorithm : public Sensor {
public:
  Algorithm();
  void MPPT_Process();
  void Control_PWM();
};


// === Class Led ===
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