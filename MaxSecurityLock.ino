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

void loop() {

  updateKeypadStates();
  buzzer.update();
  led.update();
}

void clearDigits() {
  char cleared_digits[PASSCODE_MAX_LENGTH] = {' ',' ',' ',' ',' ',' ', ' ', ' '};
  memcpy(digits, cleared_digits, PASSCODE_MAX_LENGTH);
}

void handleKeyEntry() {
  const bool isMatch = keylock.passcodeMatch(digits);
  if (currentMode == Mode::LOCKED && isMatch) {
    currentMode = Mode::UNLOCKED;
    led.setColorGreen();
    led.turnOnFor(500);
    buzzer.turnOnFor(500);
    Serial.println("** Correct Passcode **");
  } else if (currentMode == Mode::SET) {
    keylock.changePasscode(passcode, digits);
    currentMode = Mode::LOCKED;
    buzzer.turnOnFor(500);
    led.turnOff(); // Because the mode sets it to blue
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

void changePasscode() {
  currentMode = Mode::SET;
  led.setColorBlue();
  led.turnOn();
}

void updateKeypadStates() {
  char key = keypad.getKey();
  KeyState state = keypad.getState(); 

  if (key) {
    Serial.println(key);
    if (key >= '0' && key <= '9') {
      buzzer.turnOnFor(100);
      led.setColorBlue();
      led.turnOnFor(100);
      digits[passcodeIdx++] = key;
    } else if (key == '#') {
      handleKeyEntry();
      passcodeIdx = 0;
    } else if (key == '*') {
      clearDigits();
      buzzer.turnOnFor(200);
      led.setColorBlue();
      led.turnOn();
    }
  }

  if (state == HOLD && currentMode == Mode::UNLOCKED) {
    changePasscode();
    passcodeIdx = 0;
  }
}
