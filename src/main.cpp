#include <Arduino.h>
#include "Wire.h"
#include "SSD1306.h"

SSD1306 ecranOled(0x3c, 5, 4);

void setup() {
  ecranOled.init();
  ecranOled.clear();
  ecranOled.setFont(ArialMT_Plain_16);
  ecranOled.drawString(0, 10, "Mon appli v1.0");
  ecranOled.display();
}

void loop() {
  Serial.println("Bonjour!");
  sleep(2);
}
