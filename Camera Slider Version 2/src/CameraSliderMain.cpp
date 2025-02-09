#include "Config.h"

void setup() {
  Serial.begin(9600);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  setupPins();
  setupDisplay();
  attachInterrupt(digitalPinToInterrupt(pinA), PinA, RISING);
  attachInterrupt(digitalPinToInterrupt(pinB), PinB, RISING);
  displayLogo();
  Serial.println("Setup complete");
}

void loop() {
  selectMode();
  executeMode();
}