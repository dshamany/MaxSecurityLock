#include "Buzzer.h"

constexpr uint8_t COLUMNS = 2;
constexpr uint8_t ROWS = 2;
constexpr uint8_t pinColumn1 = 10;
constexpr uint8_t pinColumn2 = 9;
constexpr uint8_t pinRow1 = 21;
constexpr uint8_t pinRow2 = 20;
const int columnPins[COLUMNS] = {pinColumn1, pinColumn2};
const int rowPins[ROWS] = {pinRow1,pinRow2};
const int pins[ROWS][COLUMNS] = {{pinColumn1, pinColumn2}};
const int keypadValues[ROWS][COLUMNS] = {{1, 2}, {3, 4}};
bool keypadStates[ROWS][COLUMNS];

unsigned long now;
unsigned long nextTrigger = 0;
unsigned long loopDuration = 1000;
Buzzer buzzer(0);

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
      keypadStates[r][c] = LOW;
    }
  }

  Serial.println("BOOT!");
}

/*
All pins INPUT_PULLUP == HIGH.
Loop through COLUMNS and switch one by one to OUTPUT LOW.
If a row turns LOW as well we can pinpoint button.
*/
void loop() {
  updateKeypadStates();

  buzzer.update();
  now = millis();
  if (now >= nextTrigger) {
    buzzer.buzzFor(50);
    // buzzer.toggle();
    nextTrigger = now + loopDuration;
  }
  delay(10);
}

void updateKeypadStates() {
  for (int c = 0; c < COLUMNS; c++) {
    pinMode(columnPins[c], OUTPUT);
    digitalWrite(columnPins[c], LOW);
    for (int r = 0; r < ROWS; r++) {
      if (digitalRead(rowPins[r]) == LOW) {
        keypadStates[r][c] = HIGH;
        Serial.println(keypadValues[r][c]);
      } else keypadStates[r][c] = LOW;
    }
    pinMode(columnPins[c], INPUT_PULLUP);
  }
}