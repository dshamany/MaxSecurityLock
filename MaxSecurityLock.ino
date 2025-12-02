constexpr uint8_t COLUMNS = 3;
constexpr uint8_t ROWS = 3;
constexpr uint8_t column1 = 9;
constexpr uint8_t column2 = 8;
constexpr uint8_t column2 = 7;
constexpr uint8_t row1 = 10;
constexpr uint8_t row2 = 20;
constexpr uint8_t row3 = 21;
const int columnPins[COLUMNS] = {column1, column2, column3};
const int rowPins[ROWS] = {row1, row2, row3};
const int keypad[ROWS][COLUMNS] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

void setup() {
  Serial.begin(9600);
  delay(1500);
  for (auto& c : columnPins) {
    pinMode(c, INPUT_PULLUP);
  }
  for (auto& r : rowPins) {
    pinMode(r, OUTPUT);
    digitalWrite(r, LOW);
  }
  Serial.println("BOOT!");
}

/*
COLUMNS == INPUT_PULLUP
ROWS = OUTPUT
CHANGE!
Loop through COLUMNS and switch one by one to OUTPUT LOW.
If a row turns LOW as well we can pinpoint button.
*/
void loop() {
  bool keyFound = false;
  for (int c = 0; c < COLUMNS; c++) {
    if (digitalRead(columnPins[c]) != LOW) continue;
    for (int r = 0; r < ROWS; r++) {
      digitalWrite(rowPins[r], HIGH);
      if (digitalRead(columnPins[c]) == HIGH) {
        Serial.println(keypad[r][c]);
        keyFound = true;
      }
      digitalWrite(rowPins[r], LOW);
      if (keyFound) break;
    }
    if (keyFound) break;
  }
}