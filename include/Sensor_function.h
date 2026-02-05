#ifndef SENSOR_FUNCTION_H
#define SENSOR_FUNCTION_H

#include <Arduino.h>

class Sensor {
public:
  Sensor(uint8_t rPin, uint8_t buttonPin, uint32_t debounceDelay = 50);
  void begin();
  uint8_t read_R();
  float Convert_voltage();
  bool readbutton();

private:
  uint8_t _rPin;
  uint8_t _buttonPin;
  uint32_t _debounceDelay;
  bool _lastButtonState;
  unsigned long _lastDebounceTime;
};

class Led_state {
public:
  Led_state(uint8_t ledPin);
  void begin();
  void set(bool on);
private:
  uint8_t _ledPin;
};

#endif 