#include "function.h"
#include <Arduino.h>

// ===Class Button===
Button::Button(const Config& config) 
  : _config(config), 
    _buttonState(false),
    _lastInterruptTime(0),
    _Butonn_callback(nullptr)
  {
    // Constructor body ว่างได้ถ้า init list ครบแล้ว
  }

// ===Class Button===
void Button::button_begin(void (*button_isr)()){
  pinMode(_config.Button_Pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(_config.Button_Pin), button_isr, _config.button_edge);

  // แสดงข้อความว่าปุ่มพร้อมใช้งาน
  Serial.print("Button initialized: ");
  Serial.print(_config.Button_Name);
  Serial.print(" on GPIO ");
  Serial.println(_config.Button_Pin);
}
void Button::setCallback(void (*Button_callback)()){
  _Butonn_callback = Button_callback;
}

#ifdef ESP32
void IRAM_ATTR Button::handleInterrupt(){
  unsigned long interruptTime = millis();
  if (interruptTime - _lastInterruptTime > _config.debounceDelay) {
    
    #ifdef ESP32
    portENTER_CRITICAL_ISR(&mux);
    #endif

    _buttonState = true; // Set flag ว่ามีการกดเกิดขึ้น

    #ifdef ESP32
    portEXIT_CRITICAL_ISR(&mux);
    #endif
  
    _lastInterruptTime = interruptTime;
  }
}
#else
void ICACHE_RAM_ATTR Button::handleInterrupt(){

     unsigned long interruptTime = millis();
  if (interruptTime - _lastInterruptTime > _config.debounceDelay) {
    _lastInterruptTime = interruptTime;
    portENTER_CRITICAL_ISR(&mux);
    _buttonState = !_buttonState;
    portEXIT_CRITICAL_ISR(&mux);
    _lastInterruptTime = interruptTime;
  }
}
#endif

void Button::update(){
  bool pressed = false;
  // อ่านค่าแบบ Atomic (ใช้กับ ISR)
  #ifdef ESP32
  portENTER_CRITICAL(&mux);
  if(_buttonState){
    pressed = true;
    _buttonState = false;
  }
  portEXIT_CRITICAL(&mux);
  #else
  noInterrupts();
  if(_buttonState){
    pressed = true;
    _buttonState = false;
  }
  interrupts();
  #endif

  // ตรวจสอบว่ามี callback ถูกกำหนดไว้หรือไม่
  // nullptr = ไม่มี callback
  if(pressed && _Butonn_callback != nullptr){
    _Butonn_callback();
  }
}
const char* Button::getName() const {
  return _config.Button_Name;
}
uint8_t Button::getPin() const {
  return _config.Button_Pin;
}
bool Button::isPressed() {
  bool s = false;
  #ifdef ESP32
  portENTER_CRITICAL(&mux);
  s = _buttonState;

  _buttonState = false; // Clear flag
  portEXIT_CRITICAL(&mux);
  
  #else
  noInterrupts();
  s = _buttonState;
  _buttonState = false;
  interrupts();
  #endif
  return s;
}

// ===Class Sensor===
Sensor::Sensor() : 
  _rPin(R_PIN)  
  {
    begin();
    read_R();
    Convert_voltage();
    read_Current();
    read_temp();
  }

void Sensor::begin() {
    #ifdef ESP32
    analogReadResolution(12);
    #endif
}

uint8_t Sensor::read_R() {
  int raw = analogRead(_rPin);
#ifdef ESP32
  raw = constrain(raw, 0, 4095);
  // raw = 0-4095 -> map to 0-255
  return map(raw, 0, 4095, 0, 255);
#else
  // ESP8266: 0-1023
  raw = constrain(raw, 0, 1023);
  // raw = 0-4095 -> map to 0-255
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

// == Class led ===
Led_state::Led_state(uint8_t ledPin) : _ledPin(ledPin), _ledState(false){}

void Led_state::begin(){ 
    pinMode(_ledPin, OUTPUT);
    digitalWrite(_ledPin, HIGH); 
}
void Led_state::set(bool on) {
  digitalWrite(_ledPin, on ? LOW : HIGH); // active LOW 
}

void Led_state::toggle() {
    set(!_ledState);
}