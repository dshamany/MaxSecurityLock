#include <Keypad.h>

#include "KeyLock.h"
#include "pinout.h"

#define ROWS 2
#define COLS 2

char keyMap[ROWS][COLS] = {{'1','2'}, {'3','4'}};

constexpr uint8_t pinColumn1 = PIN_COL_1;
constexpr uint8_t pinColumn2 = PIN_COL_2;
constexpr uint8_t pinRow1 = PIN_ROW_1;
constexpr uint8_t pinRow2 = PIN_ROW_2;
uint8_t rowPins[ROWS] = {PIN_ROW_1, PIN_ROW_2};
uint8_t colPins[COLS] = {PIN_COL_1, PIN_COL_2};
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

void setup() {
  Serial.begin(9600);
  delay(100);
  Serial.println("BOOT!");
}

/*
All pins INPUT_PULLUP == HIGH.
Loop through COLUMNS and switch one by one to OUTPUT LOW.
If a row turns LOW as well we can pinpoint button.
*/
void loop() {
  updateKeypadStates();

  delay(10);


  // This is because the code below doesn't work in setup
  if (!printedDebug) {
    KeyLock keylock(passcode, passcodeLength);
    keylock.debug();
    printedDebug = true;
  }
}

void updateKeypadStates() {
  char key = keypad.getKey();
  if (key) {
    if (key >= '0' && key <= '9' && passcodeIdx < 10) {
      digits[passcodeIdx++] = key;
      Serial.print("Digit: ");
      Serial.println(key);
    } else if (key == '#') {
      digits[passcodeIdx] = '\0';
      Serial.print("Array: ");
      Serial.println(digits);
      passcodeIdx = 0;
    } else if (key == '*') {
      passcodeIdx = 0;
      Serial.println("Cleared");
    }
  }

  if (passcodeIdx == passcodeLength) {
    const bool isMatch = keylock.passcodeMatch(digits);

    // printout of current passcode
    for (int i = 0; i < passcodeLength; i++) {
      Serial.print(digits[i]);
    }
    Serial.println(" ");

    if (isMatch) {
      Serial.println("** Correct Passcode **");
    } else {
      Serial.println("-- Incorrect Passcode --");
    }
    passcodeIdx = 0;
  }
}
