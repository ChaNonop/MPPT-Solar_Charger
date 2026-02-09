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
  : sensorPins{VOLTAGE_SOLAR_PIN, VOLTAGE_BATTERY_PIN, CURRENT_PIN, TEMP_PIN},
    refVoltage(VOLTAGE_REFERENCE),
    adcResolution(ADC_VALUE),
    stete(false)
}
  {
    memset(sampleValue, 0, sizeof(sampleValue));
    memset(sensorBuffer, 0, sizeof(sensorBuffer));
}

void Sensor::begin() {
  update();
  for (uint8_t i = 0 ; i < NUM_SENSOR ; i++) {
    pinMode(sensorPins[i], INPUT);
  }
  state = true;
}

void Sensor::update() {
  for(int i = 0; i < NUM_SENSOR; i++){
    readSensorSamples(sensorPins[i], sampleBuffer, SAMPLE_COUNT);
    int avgRaw = calculateAverage(sampleBuffer, SAMPLE_COUNT);
    float voltage_solar = convertToVoltage(avgRaw);
    float voltage_battery = convertToVoltage(avgRaw);
    float current = convertToCurrent(avgRaw);
    float temp = convertToTemp(avgRaw);
  }
  // SensorData[i].rawValue = avgRaw;
  SensorData[i].Voltage_Solar = voltage;
  SensorData[i].Voltage_battery = voltage;
  SensorData[i].Current = current;
  SensorData[i].Temp = temp;
  }
}
void Sensor::readSensorSamples(uint8_t pin,int*buffer,uint8_t count){
  for (int i = 0; i < count; i++) {
    buffer[i] = analogRead(pin);
    delayMicroseconds(100); // หน่วงเวลาเล็กน้อยระหว่างการอ่าน
  }
}
int Sensor::calculateAverage(const int*sample ,uint8_t count){
  long sum = 0;
  for (int i = 0; i < count; i++) {
    sum += sample[i];
  }
  return sum / count;
}

float Sensor::convertToVoltage(int rawValue) {
  return (rawValue / adcResolution) * refVoltage;
}
uint8_t Sensor::Power() {
  return convertToVolatage * convertToCurrent;
}
uint8_t Sensor::Percentage_battery() {
  return ;
}


const SensorData* Sensor::getSensorData(uint8_t index) const {
  if (index <= NUM_SENSOR) {
    return nullptr;
  }
  return &SensorData[index];
}

const SensorData* Sensor::getAllSensorData() const {
  return SensorData;
}
void Sensor::Print_sensor() {
  for (int i = 0; i < NUM_SENSOR; i++) {
    Serial.print("Sensor ");
    Serial.print(i);
    Serial.print(": Raw Value = ");
    Serial.print(SensorData[i].rawValue);
    Serial.print(", Voltage = ");
    Serial.print(SensorData[i].Voltage);
    Serial.print(", Current = ");
    Serial.print(SensorData[i].Current);
    Serial.print(", Temp = ");
    Serial.println(SensorData[i].Temp);
  }
}
void Sensor::Display_sensor() {}

Algorithm::Algorithm() : Sensor() {}

void Algorithm::MPPT_Process(){}
void Algorithm::Control_PWM(){}




// void Sensor::Read_Sensor() {
//   int samples[Num_Pin][Num_Sample];

//   // อ่านค่า sampling NUM_SAMPLES ครั้ง
//   for (int sample = 0; sample < Num_Sample; sample++) {
//     for (int sensor = 0; sensor < Num_Pin; sensor++) {
//       samples[sensor][sample] = analogRead(Pin_sensor[sensor]);
//     }
//     delayMicroseconds(100); // หน่วงเวลาเล็กน้อยระหว่างการอ่าน
//   }
//   // ประมวลผลแต่ละ sensor
//   for (int sensor = 0; sensor < Num_Pin; sensor++) {
//     sensorValues[sensor] = NoiseReduction(samples[sensor], Num_Sample);
//   }
// }
//   // uint16_t raw_V_Battery = analogRead(Voltage_Pin_Battery);
//   // uint16_t raw_V_Solar = analogRead(Voltage_Pin_Solar);
//   // uint16_t raw_Current = analogRead(Current_Pin);
//   // uint16_t raw_Temp = analogRead(Temp_Pin);
  
//   // return raw_V_Battery, raw_V_Solar, raw_Current, raw_Temp;

// float Sensor::NoiseReduction(int*data, int size){
//   // 1. เรียงลำดับข้อมูล
//   int sortedData[Num_Sample];
//   for (int i = 0; i < size; i++) {
//     sortedData[i] = data[i];
//   }
//   bubbleSort(sortedData, size);
  
//   // 2. ตัดค่าสูงสุดและต่ำสุดออก (Trimmed Mean)
//   int trimCount = 2; // ตัดข้างละ 2 ค่า
//   long sum = 0;
//   int count = 0;
  
//   for (int i = trimCount; i < size - trimCount; i++) {
//     sum += sortedData[i];
//     count++;
//   }
  
//   // 3. คำนวณค่าเฉลี่ย
//   return (count > 0) ? (sum / count) : 0;
// }

// // ฟังก์ชันเรียงลำดับแบบ Bubble Sort
// void bubbleSort(int* arr, int size) {
// for (int i = 0; i < size - 1; i++) {
//   for (int j = 0; j < size - i - 1; j++) {
//     if (arr[j] > arr[j + 1]) {
//       int temp = arr[j];
//       arr[j] = arr[j + 1];
//       arr[j + 1] = temp;
//     }
//   }
// }
// }


// float Sensor::Convert_voltage() {
//   int raw = analogRead(_rPin);
// #ifdef ESP32
//   float v = (raw / 4095.0f) * 3.3f;
// #else
//   float v = (raw / 1023.0f) * 3.3f;
// #endif
//   return v;
// }
// float Sensor::Voltage_solar() {
//   int raw = analogRead(Voltage_Pin_Solar);
// }
// float Sensor::Voltage_battery() {
//   int raw = analogRead(Voltage_Pin_Battery);
// }
// float Sensor::read_Current() {
//   int raw = analogRead(Current_Pin);
// }
// float Sensor::read_temp() {
//   int raw = analogRead(Temp_Pin);
// }


// uint8_t Sensor::read_Current() {
//   return 0; // คืนค่า 0 ไปก่อน
// }

// uint8_t Sensor::read_temp() {
//   return 0; // คืนค่า 0 ไปก่อน
// }

// == Class led ===
// Led_state::Led_state(uint8_t ledPin) : _ledPin(ledPin), _ledState(false){}

// void Led_state::begin(){ 
//     pinMode(_ledPin, OUTPUT);
//     digitalWrite(_ledPin, HIGH); 
// }
// void Led_state::set(bool on) {
//   digitalWrite(_ledPin, on ? LOW : HIGH); // active LOW 
// }

// void Led_state::toggle() {
//     set(!_ledState);
// }