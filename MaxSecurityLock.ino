#include "thingProperties.h"
#include "SystemManager.h"

// #ifdef CLOSED
// #undef CLOSED
// #endif

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("BOOT!");
  SystemManager::setup();

  initProperties();

  ArduinoCloud.addCallback(ArduinoIoTCloudEvent::CONNECT, SystemManager::onCloudConnect);
  
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

}

void loop() {
  ArduinoCloud.update();
  
  SystemManager::run();
}
