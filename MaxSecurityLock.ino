#include "Buzzer.h"
#include "Led.h"

// Buttons
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

// Led & Buzzer
constexpr uint8_t buzzPin = 0;
constexpr uint8_t ledPinR = 5;
constexpr uint8_t ledPinG = 6;
constexpr uint8_t ledPinB = 7;
unsigned long now;
unsigned long nextLedTrigger = 0;
unsigned long ledLoopDuration = 1000;
unsigned long nextBuzzerTrigger = 0;
unsigned long buzzerLoopDuration = 1000;

Buzzer buzzer(buzzPin);
Led led(ledPinR, ledPinG, ledPinB);

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

void loop() {
  updateKeypadStates();
  static int color = 0;

  now = millis();

  buzzer.update();
  if (now >= nextBuzzerTrigger) {
    buzzer.turnOnFor(50);
    nextBuzzerTrigger = now + buzzerLoopDuration;
  }

  led.update();
  if (now >= nextLedTrigger) {
    Serial.println(color);
    if (color == 0) led.setColorRed();
    if (color == 1) led.setColorGreen();
    if (color == 2) led.setColorBlue();
    color = (color + 1) % 3;
    led.toggle();
    nextLedTrigger = now + ledLoopDuration;
  }
  delay(10);
}

void updateKeypadStates() {
  /*
  All pins INPUT_PULLUP == HIGH.
  Loop through COLUMNS and switch one by one to OUTPUT LOW.
  If a row turns LOW as well we can pinpoint button.
  */
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