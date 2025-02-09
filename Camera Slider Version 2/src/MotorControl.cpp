#include "Config.h"

int calcTravelPulses() {
  return travDist * pulsesPerMM;
}

int calcRotationPulses() {
  return rotAngle * pulsesPerDeg;
}

boolean checkRot(int i) {
  boolean rotP = false;
  float deltaAngle = 0;
  if (((travDist / 2) - (i / pulsesPerMM)) > 0) {
    if (currentAngle < 90 - (1 / pulsesPerDeg)) {
      float newAngle = atan((objDist) / ((travDist / 2) - (i / pulsesPerMM))) * 180 / M_PI;
      deltaAngle = newAngle - currentAngle;
    }
  } else if (((travDist / 2) - (i / pulsesPerMM)) < 0) {
    if (currentAngle > 0) {
      float newAngle = atan((objDist) / ((i / pulsesPerMM) - (travDist / 2))) * 180 / M_PI;
      deltaAngle = currentAngle - newAngle;
    }
  }
  if (deltaAngle >= (1 / pulsesPerDeg)) {
    rotP = true;
    currentAngle += (travDist / 2) - (i / pulsesPerMM) > 0 ? (1 / pulsesPerDeg) : -(1 / pulsesPerDeg);
  }
  return rotP;
}

float calcInterval(int numPulses) {
  return travTime > 330 ? (travTime * 1000 / numPulses) : (travTime * 1000000 / numPulses);
}

float calcRotInterval(int numPulses) {
  return travTime * 1000 / numPulses;
}

void moveToPosition() {
  int pulses;
  float interval = 1000000.0 / defaultSpeed; // Default speed in pulses per second

  switch (dataInputNo) {
    case 0: // Travel direction
      digitalWrite(travDirPin, travelDir == 0 ? LOW : HIGH);
      break;
    case 1: // Travel distance
      pulses = abs(travDist - prevTravDist) * pulsesPerMM;
      if (travDist < prevTravDist) {
        digitalWrite(travDirPin, HIGH);
      } else {
        digitalWrite(travDirPin, LOW);
      }
      moveMotor(pulses, interval, travStepPin, travDirPin, leftLimitSwitch, rightLimitSwitch);
      prevTravDist = travDist;
      break;
    case 2: // Rotation direction
      digitalWrite(rotDirPin, rotDir == 0 ? HIGH : LOW);
      break;
    case 3: // Rotation angle
      pulses = abs(rotAngle - prevRotAngle) * pulsesPerDeg;
      if (rotAngle < prevRotAngle) {
        digitalWrite(rotDirPin, LOW);
      } else {
        digitalWrite(rotDirPin, HIGH);
      }
      moveMotor(pulses, interval, rotStepPin, rotDirPin);
      prevRotAngle = rotAngle;
      break;
    default:
      break;
  }
}