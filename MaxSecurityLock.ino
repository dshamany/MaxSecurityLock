#include <Keypad.h>

#include "Buzzer.h"
#include "Led.h"
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
Mode currentMode = Mode::SET;
KeyLock keylock(passcode, passcodeLength);

bool printedDebug = false;

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
  Serial.println("BOOT!");

  if (currentMode == Mode::SET) {
    Serial.println(" ");
    Serial.println("## Set Password ##");
  }
}

static int color = 0;

void loop() {
  updateKeypadStates();

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
  char key = keypad.getKey();
  if (key) {
    if (key >= '0' && key <= '9' && passcodeIdx < 10) {
      digits[passcodeIdx++] = key;
      // Serial.print("Digit: ");
      // Serial.println(key);
    } else if (key == '#') {
      digits[passcodeIdx] = '\0';
      // Serial.print("Array: ");
      Serial.println(digits);
      passcodeIdx = 0;
    } else if (key == '*') {
      passcodeIdx = 0;
      Serial.println("Cleared");
    }
  }

  switch (currentMode){
    case Mode::LOCKED:
      if (passcodeIdx == passcodeLength) {
        const bool isMatch = keylock.passcodeMatch(digits);
        if (isMatch) {
          Serial.println("** Correct Passcode **");
        } else {
          Serial.println("-- Incorrect Passcode --");
        }
        passcodeIdx = 0;
      }
      break;
    case Mode::SET:
      if (passcodeIdx == passcodeLength) {
        keylock.changePasscode(passcode, digits);
        currentMode = Mode::LOCKED;
        Serial.println("## Password Set ##");
        passcodeIdx = 0;
      }
  }
}
