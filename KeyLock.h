#pragma once

#include <cstddef>

constexpr std::size_t PASSCODE_MAX_LENGTH = 8;

class KeyLock {
private:
  int _passcode_length;
  int _passcode[PASSCODE_MAX_LENGTH];

public:
  KeyLock() = delete;
  KeyLock(const int (&passcode)[PASSCODE_MAX_LENGTH],
          std::size_t passcode_length = PASSCODE_MAX_LENGTH);
  bool passcodeMatch(const int (&input)[PASSCODE_MAX_LENGTH]);
  bool changePasscode(const int (&previous)[PASSCODE_MAX_LENGTH],
                      const int (&new_pass)[PASSCODE_MAX_LENGTH]);
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
