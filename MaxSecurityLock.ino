constexpr uint8_t COLUMNS = 3;
constexpr uint8_t ROWS = 3;
constexpr uint8_t column1 = 9;
constexpr uint8_t column2 = 8;
constexpr uint8_t column3 = 7;
constexpr uint8_t row1 = 10;
constexpr uint8_t row2 = 20;
constexpr uint8_t row3 = 21;
const int columnPins[COLUMNS] = {column1, column2, column3};
const int rowPins[ROWS] = {row1, row2, row3};
const int keypad[ROWS][COLUMNS] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
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
  Serial.println(keypad[r][c]);
  delay(10);
}

void updateKeypadStates() {
  for (int c = 0; c < COLUMNS; c++) {
    pinMode(columnPins[c], OUTPUT);
    digitalWrite(columnPins[c], LOW);
    for (int r = 0; r < ROWS; r++) {
      if (digitalRead(rowPins[r]) == LOW) {
        keypadStates[r][c] = HIGH;
      } else keypadStates[r][c] = LOW;
    }
    pinMode(columnPins[c], INPUT_PULLUP);
  }
}