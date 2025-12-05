#include "Led.h"  

Led::Led(uint8_t pinR, uint8_t pinG, uint8_t pinB)
  : _pinR(pinR), _pinG(pinG) ,_pinB(pinB), _state(false), _timeOff(0), _timerOn(false), _r(HIGH), _g(HIGH), _b(HIGH) {
  pinMode(_pinR, OUTPUT);
  pinMode(_pinG, OUTPUT);
  pinMode(_pinB, OUTPUT);
}

void Led::turnOn() {
  _state = true;
  digitalWrite(_pinR, _r);
  digitalWrite(_pinG, _g);
  digitalWrite(_pinB, _b);
}

void Led::turnOff() {
  _state = false;
  _r = LOW;
  _g = LOW;
  _b = LOW;
  digitalWrite(_pinR, _r);
  digitalWrite(_pinG, _g);
  digitalWrite(_pinB, _b);
}

void Led::turnOnFor(uint16_t duration) {
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

void Led::setColorRed() {
  _r = HIGH;
  _g = LOW;
  _b = LOW;
}

void Led::setColorGreen() {
  _r = LOW;
  _g = HIGH;
  _b = LOW;
}

void Led::setColorBlue() {
  _r = LOW;
  _g = LOW;
  _b = HIGH;
}