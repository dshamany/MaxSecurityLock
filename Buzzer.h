#pragma once
#include <Arduino.h>

class Buzzer {
  uint8_t _pin;
  bool _state;
  unsigned long _timeOff;
  bool _timerOn;
public:
  Buzzer(uint8_t pin);
  void buzzFor(uint16_t duration);
  void turnOn();
  void turnOff();
  void toggle();
  void update();
};