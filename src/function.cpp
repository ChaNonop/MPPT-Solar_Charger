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

void IRAM_ATTR Button::handleInterrupt(){
  unsigned long interruptTime = millis();
  if (interruptTime - _lastInterruptTime > _config.debounceDelay) {
    portENTER_CRITICAL_ISR(&mux);
    _buttonState = true; // Set flag ว่ามีการกดเกิดขึ้น
    portEXIT_CRITICAL_ISR(&mux);
    _lastInterruptTime = interruptTime;
  }
}

void Button::update(){
  bool pressed = false;
  // อ่านค่าแบบ Atomic (ใช้กับ ISR)
  portENTER_CRITICAL(&mux);
  if(_buttonState){
    pressed = true;
    _buttonState = false;
  }
  portEXIT_CRITICAL(&mux);
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
  portENTER_CRITICAL(&mux);
  s = _buttonState;

  _buttonState = false; // Clear flag
  portEXIT_CRITICAL(&mux);
}

// ===Class Sensor===
Sensor::Sensor(){
  _currentData= {0.0f, 0.0f, 0.0f, 0.0f, 0, 0};
}

void Sensor::begin() {
  for (int i = 0 ; i < NUM_SENSOR ; i++) {
    pinMode(sensorPins[i], INPUT);
  }
  update();
}

void Sensor::update() {
    // 1. อ่านค่า Raw Data และหาค่าเฉลี่ย
  int rawSolar = readAverage(VOLTAGE_SOLAR_PIN, SAMPLE_COUNT);
  int rawBat   = readAverage(VOLTAGE_BATTERY_PIN, SAMPLE_COUNT);
  int rawCurr  = readAverage(CURRENT_PIN, SAMPLE_COUNT);
  int rawTemp  = readAverage(TEMP_PIN, SAMPLE_COUNT);
  
  // 2. แปลงค่า (Calibration)
  // หมายเหตุ: คูณ Multiplier ตามวงจร Voltage Divider ที่ใช้จริง
  // เช่น ถ้าใช้ R1=30k, R2=7.5k อัตราส่วนคือ 5.0 -> ต้องใส่ 5.0 เป็น argument ตัวที่สอง
  _currentData.Voltage_solar   = rawToVoltage(rawSolar, 11.0); // สมมติว่า Divider 11 เท่า (แก้ตามจริง)
  _currentData.Voltage_battery = rawToVoltage(rawBat, 5.0);    // สมมติว่า Divider 5 เท่า (แก้ตามจริง)
  _currentData.Current        = rawToCurrent(rawCurr);
  _currentData.Temp           = rawToTemp(rawTemp);
}

int Sensor::readAverage(uint8_t pin, uint8_t count) {
  long sum = 0;
  for (int i = 0; i < count; i++) {
    sum += analogRead(pin);
    delayMicroseconds(100); // รอ ADC 
  }
  return (count > 0) ? (sum / count) : 0;
}

float Sensor::rawToVoltage(int raw, float multiplier) {
  // ESP32 ADC: 0-4095 -> 0-3.3V
  float pinVoltage = (raw / (float)ADC_VALUE) * VOLTAGE_REFERENCE;
  return pinVoltage * multiplier;
}

float Sensor::rawToCurrent(int raw) {
  // ต้องแก้สมการตาม Sensor กระแสที่ใช้ (เช่น ACS712 หรือ Shunt Resistor)
  // ตัวอย่าง: (Vout - Offset) / Sensitivity
  float pinVoltage = (raw / (float)ADC_VALUE) * VOLTAGE_REFERENCE;
  return (pinVoltage - 2.5) / 0.066;
}

float Sensor::rawToTemp(int raw) {
  // ตัวอย่างแปลงค่าอุณหภูมิ (NTC หรือ LM35)
  float pinVoltage = (raw / (float)ADC_VALUE) * VOLTAGE_REFERENCE;
  return pinVoltage * 100.0; 
}

int Sensor::Power(int voltage, int current) {
  return voltage * current;
}

SensorData Sensor::getData() const {
  return _currentData
}

void Sensor::Print_sensor() {
  Serial.printf("Solar: %.2fV | Bat: %.2fV | Curr: %.2fA | Pwr: %.2fW | DutyCycle: %d%\n", 
                _currentData.Voltage_solar, 
                _currentData.Voltage_battery, 
                _currentData.Current, 
                _currentData.Power,
                _currentData.dutyCycle);
}
void Sensor::Display_sensor() {}

Algorithm::Algorithm() : Sensor() {}

void Algorithm::MPPT_Process(){}
void Algorithm::Control_PWM(){}


//== Class led ===
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