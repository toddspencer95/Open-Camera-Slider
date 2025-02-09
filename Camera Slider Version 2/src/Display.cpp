#include "Config.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setupDisplay() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
}

void displayLogo() {
  display.drawBitmap(0, 0, logo, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
  display.display();
  delay(2000);
  display.clearDisplay();
}

void updateMainMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(28, 4);
  display.print(F("Camera Slider"));
  display.setCursor(25, 20);
  display.print(F("Home Slider"));
  display.setCursor(25, 30);
  display.print(F("Pan & Rotate"));
  display.setCursor(25, 40);
  display.print(F("Point A - Point B"));

  int selected = encoderPos * 10 + 20;
  display.setCursor(14, selected);
  display.print(F(">"));
  display.display();
}

void displayLimitReached() {
  display.setCursor(28, 4);
  display.print(F("Limit Reached"));
  display.display();
}

void displayLoopInfo(int loop, const __FlashStringHelper* mode) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(30, 30);
  display.print(mode);
  display.display();
  display.setCursor(30, 40);
  display.print(F("Loop Number:"));
  display.display();
  display.setCursor(110, 40);
  display.print(loop);
  display.display();
}

void displayStart() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(45, 20);
  display.print(F("Push To"));
  display.setCursor(50, 32);
  display.print(F("Start"));
  display.display();
  waitForButtonPress();
  pinMode(enablePin, OUTPUT);
  countdown();
}

void displayEnd() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(40, 15);
  display.print(F("Complete"));
  display.setCursor(44, 35);
  display.print(F("Push To"));
  display.setCursor(27, 47);
  display.print(F("Release Motors"));
  display.display();
  waitForButtonPress();
  pinMode(enablePin, INPUT);
  resetVariables();
}

void countdown() {
  for (int i = countDown; i > 0; i--) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(60, 20);
    display.print(i);
    display.display();
    delay(1000);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(45, 15);
  display.print(F("Running"));
  display.display();
}

void updateInputDisplay() {
  switch (modeSelected) {
    case 0: updateHomeSliderDisplay(); break;
    case 1: updatePanAndRotateDataDisplay(); break;
    case 2: updatePointADataDisplay(); break;
    default: break;
  }
}

void updateHomeSliderDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(2, 2);
  display.print(F("Home Dir:  "));
  travelDir = encoderPos;
  display.setCursor(65, 2);
  display.print(F(">"));
  display.print(travelDir == 0 ? F(" Right") : F(" Left"));
  display.display();
}

// Consolidated function for distance/rotation display
void updateDistanceRotationDisplay(int travDist, int travelDir, int rotAngle, int rotDir, int dataInputNo) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(2, 2);
  display.print(F("Distance: "));
  display.setCursor(2, 12);
  display.print(F("Trav. Dir: "));
  display.setCursor(2, 22);
  display.print(F("Rot. Ang: "));
  display.setCursor(2, 32);
  display.print(F("Rot. Dir: "));

  int selected = 0;
  switch (dataInputNo) {
    case 0: selected = 2; if (!isConfirmed) travDist = encoderPos; break;
    case 1: selected = 12; if (!isConfirmed) travelDir = encoderPos; break;
    case 2: selected = 22; if (!isConfirmed) rotAngle = encoderPos; break;
    default: selected = 32; if (!isConfirmed) rotDir = encoderPos; break;
  }

  display.setCursor(65, selected);
  display.print(F(">"));
  display.setCursor(75, 2);
  display.print(travDist);
  display.print(F("mm"));
  display.setCursor(75, 12);
  display.print(travelDir == 0 ? F("Right") : F("Left"));
  display.setCursor(75, 22);
  display.print(rotAngle);
  display.print(F("deg"));
  display.setCursor(75, 32);
  display.print(rotDir == 0 ? F("CCW") : F("CW"));
  display.display();
}

// Function to handle the timing display
void updateTimingDisplay(int& numHours, int& numMinutes, int& numSeconds, int& countDown, int& numLoops, int dataInputNo) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(2, 2);
  display.print(F("Num Hours: "));
  display.setCursor(2, 12);
  display.print(F("Num Mins: "));
  display.setCursor(2, 22);
  display.print(F("Num Secs: "));
  display.setCursor(2, 32);
  display.print(F("Count Down: "));
  display.setCursor(2, 42);
  display.print(F("Num Loops: "));

  int selected = 0;
  switch (dataInputNo) {
    case 4: selected = 2; if (!isConfirmed) numHours = encoderPos; break;
    case 5: selected = 12; if (!isConfirmed) numMinutes = encoderPos; break;
    case 6: selected = 22; if (!isConfirmed) numSeconds = encoderPos; break;
    case 7: selected = 32; if (!isConfirmed) countDown = encoderPos; break;
    default: selected = 42; if (!isConfirmed) numLoops = encoderPos; break;
  }

  display.setCursor(65, selected);
  display.print(F(">"));
  display.setCursor(75, 2);
  display.print(numHours);
  display.setCursor(75, 12);
  display.print(numMinutes);
  display.setCursor(75, 22);
  display.print(numSeconds);
  display.setCursor(75, 32);
  display.print(countDown);
  display.print(F("s"));
  display.setCursor(75, 42);
  display.print(numLoops);
  display.display();
}

// Updated function for pan and rotate data display using the consolidated function
void updatePanAndRotateDataDisplay() {
  // Display distance/rotation parameters if dataInputNo is 0 to 3
  if (dataInputNo <= 3) {
    updateDistanceRotationDisplay(travDist, travelDir, rotAngle, rotDir, dataInputNo);
  } 
  // Display timing parameters if dataInputNo is 4 or greater
  else {
    updateTimingDisplay(numHours, numMinutes, numSeconds, countDown, numLoops, dataInputNo);
  }
}

// Updated function for point A data display using the consolidated function
void updatePointADataDisplay() {
  // Same logic as updatePanAndRotateDataDisplay
  if (dataInputNo <= 3) {
    updateDistanceRotationDisplay(travDist, travelDir, rotAngle, rotDir, dataInputNo);
  } 
  else {
    updateTimingDisplay(numHours, numMinutes, numSeconds, countDown, numLoops, dataInputNo);
  }
}