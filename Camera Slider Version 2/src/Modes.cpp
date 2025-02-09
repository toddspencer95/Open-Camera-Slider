#include "Config.h"

void selectMode() {
  encLowLim = 0;
  encHighLim = 4;  // Updated to reflect the new mode
  encIncrement = 1;
  updateMainMenu();
  boolean confirmed = false;
  boolean pressed = false;
  encoderPos = 0;

  while (!confirmed) {
    byte buttonState = digitalRead(encButton);
    if (buttonState != oldButtonState) {
      if (millis() - buttonPressTime >= debounceTime) {
        buttonPressTime = millis();
        oldButtonState = buttonState;
        if (buttonState == LOW) {
          modeSelected = encoderPos;
          pressed = true;
          Serial.println("Button Pushed");
        } else {
          if (pressed) {
            confirmed = true;
            Serial.println("Mode confirmed");
          }
        }
      }
    }
    if (encoderPos != prevEncoderPos) {
      updateMainMenu();
      prevEncoderPos = encoderPos;
    }
  }
  Serial.println("Mode selected");
}

void executeMode() {
  switch (modeSelected) {
    case 0: runHomeSlider(); break;
    case 1: runPanAndRotate(); break;
    case 2: runPointAToPointB(); break;
    default: break;
  }
}

void runHomeSlider() {
  inputHomeSliderData();
  displayStart();
  display.setCursor(35, 30);
  display.print(F("Homing Slider"));
  display.display();
  travelDir = toggleDirection(travelDir, travDirPin);
  moveMotor(25000, 380, travStepPin, travDirPin, leftLimitSwitch, rightLimitSwitch);
  displayEnd();
}

void runPanAndRotate() {
  inputPanAndRotateData();
  setTiming();
  displayStart();
  travTime = numHours * 3600 + numMinutes * 60 + numSeconds;

  if (travTime != 0) {
    for (int i = 1; i <= numLoops; i++) {
      displayLoopInfo(i, F("Pan & Rotate"));

      travelDir = toggleDirection(travelDir, travDirPin);
      rotDir = toggleDirection(rotDir, rotDirPin);

      int travelPulses = calcTravelPulses();
      int rotationPulses = calcRotationPulses();
      float interval = calcInterval(travelPulses);

      if (travelPulses != 0 && rotationPulses == 0) {
        moveMotor(travelPulses, interval, travStepPin, travDirPin, leftLimitSwitch, rightLimitSwitch);
      } else if (travelPulses == 0 && rotationPulses != 0) {
        moveMotor(rotationPulses, interval, rotStepPin, rotDirPin);
      } else {
        movePanAndRotate(travelPulses, rotationPulses, interval, travStepPin, rotStepPin, travDirPin, rotDirPin, leftLimitSwitch, rightLimitSwitch);
      }
    }
  }
  displayEnd();
}

void runPointAToPointB() {
  setPointB();
  setPointA();
  setTiming();
  displayStart();
  int travelPulses = calcTravelPulses();
  int rotationPulses = calcRotationPulses();
  float interval = calcInterval(travelPulses);

  for (int i = 1; i <= numLoops; i++) {
    displayLoopInfo(i, F("Point A - Point B"));
    movePanAndRotate(travelPulses, rotationPulses, interval, travStepPin, rotStepPin, travDirPin, rotDirPin, leftLimitSwitch, rightLimitSwitch);
  }
  displayEnd();
}

void setPointB() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(30, 30);
  display.print(F("Set Point B"));
  display.display();

  pinMode(enablePin, INPUT);  // Disable motors
  waitForButtonPress();       // Wait for user to set Point B
}

void setPointA() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(30, 30);
  display.print(F("Set Point A"));
  display.display();
  delay(2000);
  pinMode(enablePin, OUTPUT);  // Enable motors

  dataInputNo = 0;
  inputPointAData();
}

void setTiming() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(30, 20);
  display.print(F("Set Timing"));
  display.display();

  dataInputNo = 4;
  inputField(numHours, numHoursMin, numHoursMax, numHoursInc);
  dataInputNo = 5;
  inputField(numMinutes, numMinutesMin, numMinutesMax, numMinutesInc);
  dataInputNo = 6;
  inputField(numSeconds, numSecondsMin, numSecondsMax, numSecondsInc);
  dataInputNo = 7;
  inputField(minCountDown, minCountDown, maxCountDown, countDownInc);
  dataInputNo = 8;
  inputField(loopMin, loopMin, loopMax, loopInc);
}

void resetVariables() {
  travDist = 0;
  travTime = 120;
  objDist = 200;
  travelDir = 0;
  rotDir = 0;
  rotAngle = 0;
  countDown = 5;
  numLoops = 1;
  numHours = 0;
  numMinutes = 0;
  numSeconds = 0;
  currentAngle = 0;
  currentDist = 0;
  prevTravDist = 0;
  prevRotAngle = 0;
}

void inputHomeSliderData() {
  dataInputNo = 0;
  inputField(0, 0, 1, 1);
}

void inputPanAndRotateData() {
  dataInputNo = 0;
  inputField(0, minTravDist, maxTravDist, travDistInc);
  dataInputNo = 1;
  inputField(0, 0, 1, 1);
  dataInputNo = 2;
  inputField(initialRotAng, minRotAng, maxRotAng, rotAngInc);
  dataInputNo = 3;
  inputField(0, 0, 1, 1);
}

void inputPointAData() {
  dataInputNo = 0;
  inputField(0, 0, 1, 1);  // Travel direction
  dataInputNo = 1;
  inputField(0, minTravDist, maxTravDist, travDistInc);  // Travel distance
  dataInputNo = 2;
  inputField(0, 0, 1, 1);  // Rotation direction
  dataInputNo = 3;
  inputField(initialRotAng, minRotAng, maxRotAng, rotAngInc);  // Rotation angle
}

void inputField(int initialSetting, int lowerLimit, int upperLimit, int increment) {
  encLowLim = lowerLimit;
  encHighLim = upperLimit;
  encIncrement = increment;
  encoderPos = initialSetting;
  prevEncoderPos = encoderPos + 1;
  boolean confirmed = false;
  boolean pressed = false;

  while (!confirmed) {
    byte buttonState = digitalRead(encButton);
    if (buttonState != oldButtonState) {
      if (millis() - buttonPressTime >= debounceTime) {
        buttonPressTime = millis();
        oldButtonState = buttonState;
        if (buttonState == LOW) {
          pressed = true;
        } else {
          if (pressed) {
            confirmed = true;
            Serial.println("Input Confirmed");
          }
        }
      }
    }
    if (encoderPos != prevEncoderPos) {
      updateInputDisplay();
      prevEncoderPos = encoderPos;
      moveToPosition();
    }
  }
}