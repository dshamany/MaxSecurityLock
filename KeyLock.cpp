#include "KeyLock.h"
#include <Arduino.h>
#include <cstddef>
#include <cstring>

KeyLock::KeyLock(const char (&passcode)[PASSCODE_MAX_LENGTH],
                 std::size_t passcode_length)
    : _passcode_length(passcode_length) {

  std::memset(this->_passcode, -1, sizeof(this->_passcode));

  // Copying one digit at a time
  for (int i = 0; i < PASSCODE_MAX_LENGTH; i++) {
    _passcode[i] = passcode[i];
  }
}

bool KeyLock::passcodeMatch(const char (&input)[PASSCODE_MAX_LENGTH]) {

  // Copying one digit at a time
  for (int i = 0; i < PASSCODE_MAX_LENGTH; i++) {
    if (input[i] != this->_passcode[i]) {
      return false;
    }
  }

  return true;
}

bool KeyLock::passcodeCompare(const char *p1, const char *p2) {

  // Copying one digit at a time
  for (int i = 0; i < PASSCODE_MAX_LENGTH; i++) {
    if (p1[i] != p2[i]) {
      return false;
    }
  }
  return true;
}

bool KeyLock::changePasscode(char (&previous)[PASSCODE_MAX_LENGTH],
                             const char (&new_pass)[PASSCODE_MAX_LENGTH]) {
  if (this->passcodeMatch(previous)) {

    // Copying one digit at a time
    for (int i = 0; i < this->_passcode_length; i++) {
      _passcode[i] = new_pass[i];
    }

    memcpy(previous, new_pass, PASSCODE_MAX_LENGTH);
  } else {
    return false;
  }

  return true;
}


bool KeyLock::try_lock() { return true; }

bool KeyLock::try_unlock() { return true; }

void KeyLock::debug() {
  Serial.println(" ");
  Serial.print("Passcode Length: ");
  Serial.println(this->_passcode_length);

  Serial.print("Passcode is ");
  for (int i = 0; i < this->_passcode_length; i++) {
      Serial.print(this->_passcode[i]);
  }
  Serial.println(" ");
}
