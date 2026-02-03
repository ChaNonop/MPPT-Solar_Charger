#include "Sensor_function.h"
#include "Pin.h"
#include <Arduino.h>


Sensor::Sensor(uint8_t rPin, uint8_t buttonPin, uint32_t debounceDelay)
  : _rPin(rPin), _buttonPin(buttonPin), _debounceDelay(debounceDelay),
    _lastButtonState(HIGH), _lastDebounceTime(0) {}

void Sensor::begin() {
  pinMode(_buttonPin, INPUT_PULLUP);
    
    #ifdef ESP32
        analogReadResolution(12);
    #endif
}

uint8_t Sensor::read_R() {
  int raw = analogRead(_rPin);
#ifdef ESP32
  // ESP32 analogRead maybe 0-4095
  raw = constrain(raw, 0, 4095);
  return map(raw, 0, 4095, 0, 255);
#else
  // ESP8266: 0-1023
  raw = constrain(raw, 0, 1023);
  return map(raw, 0, 1023, 0, 255);
#endif
}

float Sensor::Convert_voltage() {
  int raw = analogRead(_rPin);
#ifdef ESP32
  float v = (raw / 4095.0f) * 3.3f;
#else
  float v = (raw / 1023.0f) * 3.3f;
#endif
  return v;
}

bool Sensor::readbutton() {
  bool reading = digitalRead(_buttonPin);
  if (reading != _lastButtonState) {
    _lastDebounceTime = millis();
  }
  if ((millis() - _lastDebounceTime) > _debounceDelay) {
    // stable
    _lastButtonState = reading;
  }
  return (_lastButtonState == LOW); // pressed active LOW
}

// led class 
Led_state::Led_state(uint8_t ledPin) : _ledPin(ledPin) {}

void Led_state::begin(){ 
    pinMode(_ledPin, OUTPUT);
    digitalWrite(_ledPin, HIGH); // OFF if active LOW (adjust as needed)
}
void Led_state::set(bool on) {
  digitalWrite(_ledPin, on ? LOW : HIGH); // active LOW by default
}