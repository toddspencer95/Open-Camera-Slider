#include "Config.h"

void PinA() {
  cli();
  reading = PIND & 0xC;
  if (reading == B00001100 && aFlag) {
    if (encoderPos <= (encHighLim - encIncrement))
      encoderPos += encIncrement;
    else
      encoderPos = encHighLim;
    aFlag = bFlag = 0;
  } else if (reading == B00000100) {
    bFlag = 1;
  }
  sei();
}

void PinB() {
  cli();
  reading = PIND & 0xC;
  if (reading == B00001100 && bFlag) {
    if (encoderPos >= (encLowLim + encIncrement))
      encoderPos -= encIncrement;
    else
      encoderPos = encLowLim;
    aFlag = bFlag = 0;
  } else if (reading == B00001000) {
    aFlag = 1;
  }
  sei();
}

int toggleDirection(int dir, int pin) {
  digitalWrite(pin, dir ? HIGH : LOW);
  return dir;
}

void moveMotor(int pulses, float interval, int stepPin, int dirPin, int limitSwitch1, int limitSwitch2) {
  for (int i = 1; i <= pulses; i++) {
    if ((limitSwitch1 == -1 || digitalRead(limitSwitch1)) && (limitSwitch2 == -1 || digitalRead(limitSwitch2))) {
      pulseMotor(stepPin, interval);
    } else {
      displayLimitReached();
      backOffMotor(stepPin, dirPin, interval);
      break;
    }
  }
}

void movePanAndRotate(int travelPulses, int rotationPulses, float interval, int travStepPin, int rotStepPin, int travDirPin, int rotDirPin, int leftLimitSwitch, int rightLimitSwitch) {
  int travelPerRotation = travelPulses / rotationPulses;
  for (int i = 1; i <= travelPulses; i++) {
    if (digitalRead(leftLimitSwitch) && digitalRead(rightLimitSwitch)) {
      pulseMotor(travStepPin, interval);
      if (i % travelPerRotation == 0) {
        pulseMotor(rotStepPin, interval);
      }
    } else {
      displayLimitReached();
      backOffMotor(travStepPin, travDirPin, interval);
      break;
    }
  }
}

void pulseMotor(int pin, float interval) {
  digitalWrite(pin, HIGH);
  delayMicroseconds(interval / 2);
  digitalWrite(pin, LOW);
  delayMicroseconds(interval / 2);
}

void backOffMotor(int pin, int dirPin, float interval) {
  int travelDir = digitalRead(dirPin);
  digitalWrite(dirPin, !travelDir);
  for (int j = 1; j < 100; j++) {
    pulseMotor(pin, interval);
  }
  digitalWrite(dirPin, travelDir);
}

void waitForButtonPress() {
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
          }
        }
      }
    }
  }
}

// Function to handle confirmation of a selection
void confirmSelection() {
  // Confirm the current parameter and move to the next one
  isConfirmed = true;  // Lock the current parameter
  dataInputNo++;       // Move to the next parameter

  // If we move past the last timing parameter, reset
  if (dataInputNo > 7) {
    dataInputNo = 0;
  }

  // Reset confirmation for the next parameter
  isConfirmed = false;
}