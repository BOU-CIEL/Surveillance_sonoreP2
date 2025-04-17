#include <Arduino.h>
#include "Wire.h"
#include "SSD1306.h"


SSD1306 ecranOled(0x3c, 5, 4);

void setup() {
  Serial.begin(9600);

  ecranOled.init();
  ecranOled.clear();
  ecranOled.setFont(ArialMT_Plain_16);
  ecranOled.drawString(0, 10, "Mon appli v1.0");
  ecranOled.display();

  son.Setup();
  
}

void loop() {
  son.SamplesDmaAcquisition();
  Serial.print("niveauSonoreMoyen");
  Serial.println(son.niveauSonoreMoyen);
  Serial.print("niveauSonoreCrete");
  Serial.println(son.niveauSonoreCrete);
}
