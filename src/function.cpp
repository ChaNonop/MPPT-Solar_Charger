#include "Sensor_function.h"
#include "Pin.h"
#include <Arduino.h>

Button::Button(const Config& config) 
  : _config(config), _buttonState(false), _lastInterruptTime(0) 
    pressed(false),
    _lastInterruptTime(0),
    callback(nullptr),
    mux(portMUX_INITIALIZER_UNLOCKED) {}
  {

  }
// ===Class Button===
void Button::button_begin(void (*button_isr)()){
  pinMode(_config.Button_Pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(_config.Button_Pin), button_isr, _config.button_state);

  // แสดงข้อความว่าปุ่มพร้อมใช้งาน
  Serial.print("Button initialized: ");
  Serial.print(config.name);
  Serial.print(" on GPIO ");
  Serial.println(config.pin);
}
void Button::setCallback(void (*callback)()){
  this->callback = callback;
}
void IRAM_ATTR Button::handleInterrupt(){
    unsigned long interruptTime = millis();
  if (interruptTime - _lastInterruptTime > _config.debounceDelay) {
    _lastInterruptTime = interruptTime;
    portENTER_CRITICAL_ISR(&mux);
    _buttonState = !_buttonState;
    portEXIT_CRITICAL_ISR(&mux);
    _lastInterruptTime = interruptTime;
  }
}
void Button::update(){
  if(_pressed){
    portENTER_CRITICAL(&mux);
    _pressed = false;
    portEXIT_CRITICAL(&mux);

    // ตรวจสอบว่ามี callback ถูกกำหนดไว้หรือไม่
    // nullptr = ไม่มี callback
    if(callback != nullptr){
      callback();
    }
  }
}
const char* Button::getName() const {
  return _config.Button_Name;
}
uint8_t Button::getPin() const {
  return _config.Button_Pin;
}
bool Button::isPressed() const {
  return _buttonState;
}

// ===Class Sensor===
Sensor::Sensor(uint8_t rPin, uint8_t buttonPin, uint32_t debounceDelay)
  : _rPin(rPin), _buttonPin(buttonPin), _debounceDelay(debounceDelay),
    _lastButtonState(HIGH), _lastDebounceTime(0) {}

void Sensor::begin() {
  // pinMode(_buttonPin, INPUT_PULLUP);
    
    #ifdef ESP32
        analogReadResolution(12);
    #endif
}


uint8_t Sensor::read_R() {
  int raw = analogRead(_rPin);
#ifdef ESP32
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
bool Sensor::interrupt_button_set() {
  attachInterrupt(digitalPinToInterrupt(button_Pin),Sensor.readbutton,CHANGE);
  return true;
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
    digitalWrite(_ledPin, HIGH); 
}
void Led_state::set(bool on) {
  digitalWrite(_ledPin, on ? LOW : HIGH); // active LOW 
}