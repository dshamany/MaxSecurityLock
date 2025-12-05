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
constexpr uint8_t buzzPin = BUZZ_PIN;
constexpr uint8_t ledPinR = RGB_RED;
constexpr uint8_t ledPinG = RGB_GRN;
constexpr uint8_t ledPinB = RGB_BLU;
unsigned long now;
unsigned long nextLedTrigger = 0;
unsigned long ledLoopDuration = 1000;

Buzzer buzzer(buzzPin);
Led led(ledPinR, ledPinG, ledPinB);

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("BOOT!");

  if (currentMode == Mode::SET) {
    Serial.println(" ");
    Serial.println("## Set Password ##");
  }
}

static int color = 0;

void loop() {
  updateKeypadStates();
  buzzer.update();
  led.update();
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
          buzzer.turnOnFor(500);
          led.setColorGreen();
          led.turnOnFor(500);
        } else {
          Serial.println("-- Incorrect Passcode --");
          buzzer.turnOnFor(100);
          led.setColorRed();
          led.turnOnFor(100);
        }
        passcodeIdx = 0;
      }
      break;
    case Mode::SET:
      if (passcodeIdx == passcodeLength) {
        keylock.changePasscode(passcode, digits);
        currentMode = Mode::LOCKED;
        Serial.println("## Password Set ##");
        buzzer.turnOnFor(500);
        led.setColorBlue();
        led.turnOnFor(500);
        passcodeIdx = 0;
      }
  }
}
