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
  uint8_t Power();
  uint8_t Percentage_battery();
};

class Button{
private:
  Config _config;
  volatile bool _pressed;
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

// === Class Snesor ===
class Sensor {
private:
  const uint8_t sensorPins[NUM_SENSOR];
  int sampleBuffer[SAMPLE_COUNT];

  const float refVoltage = VOLTAGE_REFERENCE;
  const float adcResolution = 4095.0f;

  bool stete;
  void readSensorSamples(uint8_t pin,int*buffer,uint8_t count);
  int calculateAverage(const int*sample ,uint8_t count);

  float convertToVoltage(int rawValue);
  float convertToCurrent(int rawValue);
  float convertToTemp(int rawValue);

public:
  Sensor();
  void begin();
  void update();
  void SensorData* getSensorData(uint8_t index) const;
  void SensorData* getAllSensorData() const;

  void Print_sensor();
  void Display_sensor();
};
class Algorithm : public Sensor {
public:
  Algorithm();
  void MPPT_Process();
  void Control_PWM();
}
// class Led_state {
// public:
//   Led_state(uint8_t ledPin);
//   void begin();
//   void set(bool on);
//   void toggle();
// private:
//   uint8_t _ledPin;
//   bool _ledState;
// };

#endif 