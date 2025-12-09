#pragma once

#include <Keypad.h>

#ifdef CLOSED
#undef CLOSED
#endif

#include <Arduino.h>
#include "thingProperties.h"

#include "pinout.h"
#include "Buzzer.h"
#include "Led.h"
#include "KeyLock.h"




enum Mode {
  LOCKED,
  UNLOCKED,
  SET,
  CONFIRM
};

#define ROWS 4
#define COLS 3

char keyMap[ROWS][COLS] = {{'1','2','3'}, {'4','5','6'}, {'7','8','9'}, {'*','0','#'}};

uint8_t rowPins[ROWS] = {PIN_ROW_1, PIN_ROW_2, PIN_ROW_3, PIN_ROW_4};
uint8_t colPins[COLS] = {PIN_COL_1, PIN_COL_2, PIN_COL_3};
Keypad keypad = Keypad(makeKeymap(keyMap), rowPins, colPins, ROWS, COLS);

char clearedDigits[PASSCODE_MAX_LENGTH] = {' ',' ',' ',' ',' ',' ', ' ', ' '};
char passcode[PASSCODE_MAX_LENGTH] = { '1', '2', '3', '4', ' ', ' ', ' ', ' ' };
const int passcodeLength = 8;
int passcodeIdx = 0;
char digits[PASSCODE_MAX_LENGTH];
char confirm[PASSCODE_MAX_LENGTH];

KeyLock keylock(passcode, passcodeLength);

Mode currentMode = Mode::LOCKED;

// Led & Buzzer
unsigned long now;
unsigned long nextLedTrigger = 0;
unsigned long ledLoopDuration = 1000;

Buzzer buzzer(BUZZ_PIN);
Led led(RGB_RED, RGB_GRN, RGB_BLU);

static int color = 0;
unsigned long autoLockDurationMs = 10'000UL;

struct SystemManager {
  
  static void clearDigits() {
    passcodeIdx = 0;
    memcpy(digits, clearedDigits, PASSCODE_MAX_LENGTH);
  }

  static void clearConfirm() {
    passcodeIdx = 0;
    memcpy(confirm, clearedDigits, PASSCODE_MAX_LENGTH);
  }

  static void handleKeyEntry(char key) {

    if (currentMode == Mode::CONFIRM) {
      confirm[passcodeIdx++] = key;
    } else {
      digits[passcodeIdx++] = key;
    }

    buzzer.turnOnFor(100);
    led.setColorBlue();
    led.turnOnFor(100);
  }

  static void handleSubmission() {
    const bool isMatch = keylock.passcodeMatch(digits);
    if (currentMode == Mode::LOCKED && isMatch) {
      currentMode = Mode::UNLOCKED;
      autoLockDurationMs = millis();
      led.setColorGreen();
      led.turnOnFor(500);
      buzzer.turnOnFor(500);
      Serial.println("** Correct Passcode **");
      // Update numUnlocks on Arduino Cloud
      isLocked = false;
      numUnlocks += 1;
    } else if (currentMode == Mode::SET) {
      buzzer.turnOnFor(500);
      led.setColorBlue();
      led.turnOnFor(500);
      confirmPasscode();
    } else if (currentMode == Mode::CONFIRM && KeyLock::passcodeCompare(digits, confirm)) {
      keylock.changePasscode(passcode, digits);
      lock();
      buzzer.turnOnFor(500);
      led.setColorGreen();
      led.turnOnFor(500);
      Serial.println("## Passcode Set ##");
      clearConfirm();
      clearDigits();
    } else if (currentMode == Mode::CONFIRM) { 
      Serial.println("## Passcode Mismatch ##");
    } else {
      lock();
      led.setColorRed();
      led.turnOnFor(200);
      buzzer.turnOnFor(200);
      Serial.println("-- Incorrect Passcode --");
      // update numWrongAttempts in Arduino Cloud
      numWrongAttempts += 1;
    }
    passcodeIdx = 0;
  }

  static void handleClearDigits() {
    clearDigits();
    buzzer.turnOnFor(200);
    led.setColorBlue();
    led.turnOnFor(200);
    Serial.println("Cleared");
  }

  static void changePasscode() {
    clearDigits();
    clearConfirm();
    currentMode = Mode::SET;
    led.setColorBlue();
    led.turnOn();
    Serial.println("## Set Passcode ##");
  }

  static void confirmPasscode() {
    currentMode = Mode::CONFIRM;
    led.setColorBlue();
    led.turnOn();
    Serial.println("## Confirm Passcode ##");
  }

  static void lock() {
    currentMode = Mode::LOCKED;
    buzzer.turnOnFor(100);
    led.setColorRed();
    led.turnOnFor(100);
    Serial.println("Locked");
    clearDigits();
    clearConfirm();
    // Update isLocked boolean in Arduino Cloud
    isLocked = true;
  }

  static void autoLock() {
    unsigned long tenMilliSeconds = 10'000UL;
    if (millis() - autoLockDurationMs >= tenMilliSeconds && currentMode == Mode::UNLOCKED) {
      lock();
    }
  }

  static void updateKeypadStates() {
    char key = keypad.getKey();
    KeyState state = keypad.getState(); 

    if (key && state != HOLD) {
      Serial.println(key);
      if (key >= '0' && key <= '9') {
        handleKeyEntry(key);
      } else if (key == '#') {
        handleSubmission();
        passcodeIdx = 0;
      } else if (key == '*') {
        handleClearDigits();
      }
    }

    if (state == HOLD && currentMode == Mode::UNLOCKED) {
      changePasscode();
      passcodeIdx = 0;
    }
  }

  static void setup() {
    clearDigits();
    clearConfirm();
    keylock.debug();
    lock();
  }

  static void run() {
    updateKeypadStates();
    buzzer.update();
    led.update();
    autoLock();
  }

  static void onCloudConnect() {
    // e.g. light LED, print to LCD, show toast on serial, etc.
    led.setColorBlue();
    led.turnOnFor(100);
    buzzer.turnOnFor(100);
    Serial.println("Connected to Arduino Cloud");
  }

};