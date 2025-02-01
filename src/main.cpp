#include <Arduino.h>
#include "config_ble/config.hpp"

Config_ble ble;

void setup() {
  ble.initBLE();
  ble.configBLE("DNJKDNSJK");
}

void loop() {
  // put your main code here, to run repeatedly:
  ble.receiveDataFromJDY();
}

