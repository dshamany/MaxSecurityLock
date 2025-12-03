#include "Led.h"  

Led::Led(uint8_t pin) : _pin(pin), _state(false), _timeOff(0), _timerOn(false) {
  pinMode(_pin, OUTPUT);
}

void Led::turnOn() {
  _state = true;
  digitalWrite(_pin, _state);
}

void Led::turnOff() {
  _state = false;
  digitalWrite(_pin, _state);
}

void Led::lightFor(uint16_t duration) {
  this->turnOn();
  _timerOn = true;
  _timeOff = millis() + duration;
}

void Led::toggle() {
  Serial.print(_state);
  Serial.print("    ");
  if (_state) this->turnOff();
  else this->turnOn();
  Serial.println(_state);
}

void Led::update() {
  if (_timerOn && millis() >= _timeOff) {
    this->turnOff();
    _timerOn = false;
  }
}