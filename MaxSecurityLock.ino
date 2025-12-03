#include <GawiButtons>

#include "KeyLock.h"
#include "pinout.h"

constexpr uint8_t COLUMNS = 2;
constexpr uint8_t ROWS = 2;
constexpr uint8_t pinColumn1 = PIN_COL_1;
constexpr uint8_t pinColumn2 = PIN_COL_2;
constexpr uint8_t pinRow1 = PIN_ROW_1;
constexpr uint8_t pinRow2 = PIN_ROW_2;
const int columnPins[COLUMNS] = {pinColumn1, pinColumn2};
const int rowPins[ROWS] = {pinRow1,pinRow2};
const int keypadValues[ROWS][COLUMNS] = {{1, 2}, {3, 4}};
bool keypadStates[ROWS][COLUMNS];

void setup() {
  Serial.begin(9600);
  delay(100);
  for (auto& c : columnPins) {
    pinMode(c, INPUT_PULLUP);
  }
  for (auto& r : rowPins) {
    pinMode(r, INPUT_PULLUP);
  }

  for (int c = 0; c < COLUMNS; c++) {
    for (int r = 0; r < ROWS; r++) {
      keypadStates[r][c] = HIGH;
    }
  }

  const int passcode_length = 4;
  int passcode[] = { 1, 2, 3, 4, -1, -1, -1, -1 };
  KeyLock lock(passcode, passcode_length);
  lock.debug();

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
}

void updateKeypadStates() {
  for (int c = 0; c < COLUMNS; c++) {
    pinMode(columnPins[c], OUTPUT);
    digitalWrite(columnPins[c], LOW);
    for (int r = 0; r < ROWS; r++) {
      if (digitalRead(rowPins[r]) == LOW) {
        keypadStates[r][c] = LOW; // This is the button that is pressed
        Serial.println(keypadValues[r][c]);
        
      } else keypadStates[r][c] = HIGH;
    }
    pinMode(columnPins[c], INPUT_PULLUP);
  }
}
