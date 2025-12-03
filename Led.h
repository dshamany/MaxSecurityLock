#pragma once
#include <Arduino.h>

class Led {
  uint8_t _pin;
  bool _state;
  unsigned long _timeOff;
  bool _timerOn;
public:
  Led(uint8_t pin);
  void lightFor(uint16_t duration);
  void turnOn();
  void turnOff();
  void toggle();
  void update();
};