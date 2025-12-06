#include <Keypad.h>

#include "Buzzer.h"
#include "Led.h"
#include "KeyLock.h"
#include "pinout.h"

#define ROWS 4
#define COLS 3

char keyMap[ROWS][COLS] = {{'1','2','3'}, {'4','5','6'}, {'7','8','9'}, {'*','0','#'}};

uint8_t rowPins[ROWS] = {PIN_ROW_1, PIN_ROW_2, PIN_ROW_3, PIN_ROW_4};
uint8_t colPins[COLS] = {PIN_COL_1, PIN_COL_2, PIN_COL_3};
Keypad keypad = Keypad(makeKeymap(keyMap), rowPins, colPins, ROWS, COLS);

const int passcodeLength = 4;
char digits[PASSCODE_MAX_LENGTH] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
int passcodeIdx = 0;

enum Mode {
  LOCKED,
  UNLOCKED,
  SET
};

char passcode[PASSCODE_MAX_LENGTH] = { '1', '2', '3', '4', ' ', ' ', ' ', ' ' };
Mode currentMode = Mode::LOCKED;
KeyLock keylock(passcode, passcodeLength);

bool printedDebug = false;

// Led & Buzzer
unsigned long now;
unsigned long nextLedTrigger = 0;
unsigned long ledLoopDuration = 1000;

Buzzer buzzer(BUZZ_PIN);
Led led(RGB_RED, RGB_GRN, RGB_BLU);

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("BOOT!");
}

static int color = 0;
unsigned long autoLockDuration = 10000UL;

void loop() {

  updateKeypadStates();
  buzzer.update();
  led.update();
  autoLock();
}

void clearDigits() {
  char cleared_digits[PASSCODE_MAX_LENGTH] = {' ',' ',' ',' ',' ',' ', ' ', ' '};
  memcpy(digits, cleared_digits, PASSCODE_MAX_LENGTH);
}

void handleKeyEntry(char key) {
  buzzer.turnOnFor(100);
  led.setColorBlue();
  led.turnOnFor(100);
  digits[passcodeIdx++] = key;
}

void handleSubmission() {
  const bool isMatch = keylock.passcodeMatch(digits);
  if (currentMode == Mode::LOCKED && isMatch) {
    currentMode = Mode::UNLOCKED;
    autoLockDuration = millis();
    led.setColorGreen();
    led.turnOnFor(500);
    buzzer.turnOnFor(500);
    Serial.println("** Correct Passcode **");
  } else if (currentMode == Mode::SET) {
    keylock.changePasscode(passcode, digits);
    currentMode = Mode::LOCKED;
    buzzer.turnOnFor(500);
    led.setColorGreen();
    led.turnOnFor(500);
    Serial.println("## Passcode Set ##");
  } else {
    currentMode = Mode::LOCKED;
    led.setColorRed();
    led.turnOnFor(200);
    buzzer.turnOnFor(200);
    Serial.println("-- Incorrect Passcode --");
  }
  clearDigits();
}

void handleClearDigits() {
  clearDigits();
  buzzer.turnOnFor(200);
  led.setColorBlue();
  led.turnOnFor(200);
  Serial.println("Cleared");
}

void changePasscode() {
  currentMode = Mode::SET;
  led.setColorBlue();
  led.turnOn();
  Serial.println("## Set Passcode ##");
}

void autoLock() {
  if (millis() - autoLockDuration >= 10000UL && currentMode == Mode::UNLOCKED) {
    currentMode = Mode::LOCKED;
    buzzer.turnOnFor(100);
    led.setColorRed();
    led.turnOnFor(100);
    Serial.println("Locked");
  }
}

void updateKeypadStates() {
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
    clearDigits();
  }
}
