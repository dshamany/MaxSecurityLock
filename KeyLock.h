#pragma once

#include <Arduino.h>
#include <cstddef>

constexpr std::size_t PASSCODE_MAX_LENGTH = 8;

class KeyLock {
private:
  int _passcode_length;
  char _passcode[PASSCODE_MAX_LENGTH];

public:
  KeyLock() = delete;
  KeyLock(const char (&passcode)[PASSCODE_MAX_LENGTH],
          std::size_t passcode_length = PASSCODE_MAX_LENGTH);
  bool passcodeMatch(const char (&input)[PASSCODE_MAX_LENGTH]);
  static bool passcodeCompare(const char* input1, const char* input2);
  bool changePasscode(char (&previous)[PASSCODE_MAX_LENGTH],
                      const char (&new_pass)[PASSCODE_MAX_LENGTH]);
  bool try_lock();
  bool try_unlock();
  void debug();
};

/*
  EXPLANATION:
  The array storing the passcode can hold up to 8 integers
  so we only need to check the passcode until the end OR until
  we reach a -1
*/
// [-1, -1, -1, -1, -1, -1, -1, -1]
// [1, 2, 3, 4]
