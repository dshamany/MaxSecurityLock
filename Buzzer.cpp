#include "Buzzer.h"  

Buzzer::Buzzer(uint8_t pin) : _pin(pin), _state(false), _timeOff(0), _timerOn(false) {
  pinMode(_pin, OUTPUT);
}

void Buzzer::turnOn() {
  _state = true;
  digitalWrite(_pin, _state);
}

void Buzzer::turnOff() {
  _state = false;
  digitalWrite(_pin, _state);
}

void Buzzer::turnOnFor(uint16_t duration) {
  this->turnOn();
  _timerOn = true;
  _timeOff = millis() + duration;
}

void Buzzer::toggle() {
  Serial.print(_state);
  Serial.print("    ");
  if (_state) this->turnOff();
  else this->turnOn();
  Serial.println(_state);
}

void Buzzer::update() {
  if (_timerOn && millis() >= _timeOff) {
    this->turnOff();
    _timerOn = false;
  }
}