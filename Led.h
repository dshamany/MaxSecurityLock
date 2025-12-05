#pragma once
#include <Arduino.h>

class Led {
  uint8_t _pinR;
  uint8_t _pinG;
  uint8_t _pinB;
  bool _state;
  unsigned long _timeOff;
  bool _timerOn;
  bool _r;
  bool _g;
  bool _b;
public:
  Led(uint8_t pinR, uint8_t pinG, uint8_t pinB);
  void turnOnFor(uint16_t duration);
  void turnOn();
  void turnOff();
  void toggle();
  void update();
  void setColorRed();
  void setColorGreen();
  void setColorBlue();
};