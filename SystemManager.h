#pragma once

#include "KeyLock.h";

enum Mode {
  LOCKED,
  UNLOCKED,
  SET
};

struct SystemManager {
  Mode mode;
  char digits[PASSCODE_MAX_LENGTH];

  void run() {

  }

}