#include "SystemManager.h"

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("BOOT!");
}

void loop() {
  SystemManager::run();
}
