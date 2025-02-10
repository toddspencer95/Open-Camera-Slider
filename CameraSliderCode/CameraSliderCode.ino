#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const unsigned char logo[] PROGMEM = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x83, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xfc, 0x7f, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x3f, 0xc0, 0x3f, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x7c, 0x00, 0x1f, 0xfc, 0x0f, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xf8, 0x00, 0x0f, 0xfc, 0x0f, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xe0, 0x00, 0x0f, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xe0, 0x00, 0x07, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf3, 0xc0, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf3, 0x80, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf3, 0x80, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf3, 0x80, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x83, 0x80, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0x80, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x33, 0xc0, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x71, 0xc0, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x79, 0xe0, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x78, 0xf0, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x78, 0xf8, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x78, 0x7e, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x30, 0x1f, 0xfe, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xfd, 0xfe, 0x31, 0x0f, 0xf8, 0x4f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xfc, 0xff, 0x31, 0x80, 0x00, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xf8, 0x7f, 0x31, 0xe0, 0x03, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xf0, 0x3f, 0x39, 0xf0, 0x31, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xf0, 0x3f, 0x39, 0xf8, 0x79, 0xe3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xfc, 0xff, 0x18, 0xfc, 0x19, 0xe3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xfc, 0xff, 0x18, 0x7f, 0x01, 0xe3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x18, 0x3f, 0xc1, 0xe3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x98, 0x1f, 0xf1, 0xe3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x99, 0x87, 0xf9, 0xe3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x99, 0xc1, 0xf8, 0xc3, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x99, 0xf0, 0x70, 0x03, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x89, 0xfc, 0x00, 0x13, 0xc0, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc8, 0xff, 0x01, 0xf3, 0xc0, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x3f, 0xf9, 0xf3, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x03, 0xf9, 0xf3, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x01, 0xf9, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x01, 0xf9, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x01, 0xf9, 0xf0, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x01, 0xf9, 0xf0, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0x81, 0xc0, 0x01, 0xf9, 0xf3, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xfe, 0x0f, 0x00, 0x01, 0xf9, 0xf1, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xfc, 0x38, 0x00, 0x01, 0xf9, 0xf0, 0x38, 0x7f, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xf8, 0xe0, 0x00, 0x01, 0xf9, 0xf0, 0x0e, 0x3f, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xf1, 0xc3, 0x80, 0x01, 0xf8, 0xe3, 0x87, 0x1f, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xf3, 0x8f, 0x80, 0x00, 0xf8, 0x03, 0xe3, 0x9f, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xf3, 0x9f, 0x00, 0x00, 0x70, 0x03, 0xf3, 0x9f, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xf3, 0x87, 0x00, 0x00, 0x00, 0x03, 0xc3, 0x9f, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xf3, 0xc1, 0x80, 0x00, 0x00, 0x02, 0x07, 0x9f, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xf1, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x1f, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xf8, 0xfe, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x3f, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xfc, 0x7f, 0xe0, 0x00, 0x00, 0x0f, 0xf8, 0x7f, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xfe, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0xff, 0xff, 0xff, 0xff, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x1f, 0xff, 0xff, 0xf0, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

// Constants for motor pins and limits
const int pinA = 2;
const int pinB = 3;
const int encButton = 4;
const int enablePin = 5;
const int travDirPin = 6;
const int travStepPin = 7;
const int rotDirPin = 8;
const int rotStepPin = 9;
const int leftLimitSwitch = 10;
const int rightLimitSwitch = 11;

// Debounce time for the encoder button
const unsigned long debounceTime = 10;

// Motor and encoder parameters
const float pulsesPerMM = 50;
const float pulsesPerDeg = 17.7777;
const int minInterval = 400;
const int defaultSpeed = 500;  // Default speed for real-time adjustments
volatile int encoderPos = 0;
volatile int prevEncoderPos = 0;
volatile byte aFlag = 0;
volatile byte bFlag = 0;
volatile byte reading = 0;

byte oldButtonState = HIGH;
unsigned long buttonPressTime;
int encLowLim = 0;
int encHighLim = 3;
int encIncrement = 1;
int dataInputNo = 0;
int modeSelected = 0;

int travelDir = 0;
int rotDir = 0;
float travDist = 0;
float travTime = 120;
float objDist = 200;
int rotAngle = 0;
int countDown = 5;
int numLoops = 1;
int numHours = 0;
int numMinutes = 0;
int numSeconds = 0;
float currentAngle = 0;
float currentDist = 0;
float prevTravDist = 0;  // Store the previous travel distance
int prevRotAngle = 0;    // Store the previous rotation angle

// Constants for settings
const int minTravDist = 0;
const int maxTravDist = 450;
const int travDistInc = 25;
const int initialDur = 120;
const int minDur = 10;
const int maxDur = 330;
const int durInc = 5;
const int initialRotAng = 0;
const int minRotAng = 0;
const int maxRotAng = 360;
const int rotAngInc = 10;
const int initialObjDist = 200;
const int minObjDist = 150;
const int maxObjDist = 5000;
const int objInc = 50;
const int minCountDown = 5;
const int maxCountDown = 120;
const int countDownInc = 5;
const int loopMin = 1;
const int loopMax = 10;
const int loopInc = 1;
const int numHoursMin = 0;
const int numHoursMax = 23;
const int numHoursInc = 1;
const int numMinutesMin = 0;
const int numMinutesMax = 59;
const int numMinutesInc = 1;
const int numSecondsMin = 0;
const int numSecondsMax = 59;
const int numSecondsInc = 1;

// Function prototypes
void setupPins();
void setupDisplay();
void displayLogo();
void selectMode();
void executeMode();
void PinA();
void PinB();
void updateMainMenu();
void runHomeSlider();
void runPanAndRotate();
void runPointAToPointB();
void setPointB();
void setPointA();
void setTiming();
int toggleDirection(int dir, int pin);
void moveMotor(int pulses, float interval, int stepPin, int dirPin, int limitSwitch1 = -1, int limitSwitch2 = -1);
void movePanAndRotate(int travelPulses, int rotationPulses, float interval, int travStepPin, int rotStepPin, int travDirPin, int rotDirPin, int leftLimitSwitch, int rightLimitSwitch);
void pulseMotor(int pin, float interval);
void displayLimitReached();
void backOffMotor(int pin, int dirPin, float interval);
void displayLoopInfo(int loop, const __FlashStringHelper* mode);
void displayStart();
void displayEnd();
void waitForButtonPress();
void countdown();
void resetVariables();
void inputHomeSliderData();
void inputPanAndRotateData();
void inputPointAData();
void inputField(int initialSetting, int lowerLimit, int upperLimit, int increment);
void updateInputDisplay();
void updateHomeSliderDisplay();
void updatePanAndRotateDataDisplay();
void updatePointADataDisplay();
void updateTimingDisplay();
int calcTravelPulses();
int calcRotationPulses();
boolean checkRot(int i);
float calcInterval(int numPulses);
float calcRotInterval(int numPulses);
void moveToPosition();

void setup() {
  Serial.begin(9600);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    //Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }

  setupPins();
  setupDisplay();
  attachInterrupt(digitalPinToInterrupt(pinA), PinA, RISING);
  attachInterrupt(digitalPinToInterrupt(pinB), PinB, RISING);
  displayLogo();
  //Serial.println("Setup complete");
}

void loop() {
  selectMode();
  executeMode();
}

void setupPins() {
  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);
  pinMode(leftLimitSwitch, INPUT_PULLUP);
  pinMode(rightLimitSwitch, INPUT_PULLUP);
  pinMode(encButton, INPUT_PULLUP);
  pinMode(enablePin, INPUT);
  pinMode(travDirPin, OUTPUT);
  pinMode(travStepPin, OUTPUT);
  pinMode(rotDirPin, OUTPUT);
  pinMode(rotStepPin, OUTPUT);
  digitalWrite(travDirPin, LOW);
  digitalWrite(rotDirPin, HIGH);
}

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
          //Serial.println("Button Pushed");
        } else {
          if (pressed) {
            confirmed = true;
            //Serial.println("Mode confirmed");
          }
        }
      }
    }
    if (encoderPos != prevEncoderPos) {
      updateMainMenu();
      prevEncoderPos = encoderPos;
    }
  }
  //Serial.println("Mode selected");
}

void executeMode() {
  switch (modeSelected) {
    case 0: runHomeSlider(); break;
    case 1: runPanAndRotate(); break;
    case 2: runPointAToPointB(); break;
    default: break;
  }
}

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

void runHomeSlider() {
  inputHomeSliderData();
  displayStart();
  display.setCursor(35, 30);
  display.print(F("Homing Slider"));
  display.display();
  if (travelDir == 0)  //Set motor travel direction and then flip direction for back off when reaching the end of the slider
  {
    digitalWrite(travDirPin, LOW);
    travelDir = 1;
  } else {
    digitalWrite(travDirPin, HIGH);
    travelDir = 0;
  }
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

      // Set motor travel direction
      // Then flip for next loop
      if (travelDir == 0) {
        digitalWrite(travDirPin, LOW);
        travelDir = 1;
      } else {
        digitalWrite(travDirPin, HIGH);
        travelDir = 0;
      }
      if (rotDir == 0)  //Set motor travel direction
      {
        digitalWrite(rotDirPin, HIGH);
        rotDir = 1;
      } else {
        digitalWrite(rotDirPin, LOW);
        rotDir = 0;
      }

      int travelPulses = calcTravelPulses();
      int rotationPulses = calcRotationPulses();
      float interval = calcInterval(travelPulses);
      float rotation_interval = calcInterval(rotationPulses);

      // This is for Pan Only
      if (travelPulses != 0 && rotationPulses == 0) {
        moveMotor(travelPulses, interval, travStepPin, travDirPin, leftLimitSwitch, rightLimitSwitch);
      
      // This is for Rotate Ony
      // Notice that rotation interval is different from travel interval
      } else if (travelPulses == 0 && rotationPulses != 0) {
        moveMotor(rotationPulses, rotation_interval, rotStepPin, rotDirPin, leftLimitSwitch, rightLimitSwitch);
      
      // This is for Pan and Rotate
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
  travTime = numHours * 3600 + numMinutes * 60 + numSeconds;

  if (travTime != 0) {
    for (int i = 1; i <= numLoops; i++) {
      displayLoopInfo(i, F("Pnt A - Pnt B"));

      // Flip direction then set motor travel direction
      // Since user sets end point first, then start
      if (travelDir == 0) {
        travelDir = 1;
        digitalWrite(travDirPin, LOW);
      } else {
        travelDir = 0;
        digitalWrite(travDirPin, HIGH);
      }
      if (rotDir == 0)  //Set motor travel direction
      {
        rotDir = 1;
        digitalWrite(rotDirPin, HIGH);
      } else {
        rotDir = 0;
        digitalWrite(rotDirPin, LOW);
      }

      int travelPulses = calcTravelPulses();
      int rotationPulses = calcRotationPulses();
      float interval = calcInterval(travelPulses);
      float rotation_interval = calcInterval(rotationPulses);

      // This is for Pan Only
      if (travelPulses != 0 && rotationPulses == 0) {
        moveMotor(travelPulses, interval, travStepPin, travDirPin, leftLimitSwitch, rightLimitSwitch);
      
      // This is for Rotate Ony
      // Notice that rotation interval is different from travel interval
      } else if (travelPulses == 0 && rotationPulses != 0) {
        moveMotor(rotationPulses, rotation_interval, rotStepPin, rotDirPin, leftLimitSwitch, rightLimitSwitch);
      
      // This is for Pan and Rotate
      } else {
        movePanAndRotate(travelPulses, rotationPulses, interval, travStepPin, rotStepPin, travDirPin, rotDirPin, leftLimitSwitch, rightLimitSwitch);
      }
    }
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
  //Calculate how much the camera should pan for each rotation step
  int travelPerRotation = travelPulses / rotationPulses;
  for (int i = 1; i <= travelPulses; i++) {
    if (digitalRead(leftLimitSwitch) == true && digitalRead(rightLimitSwitch) == true) {  //limit switches not reached
      digitalWrite(travStepPin, HIGH);
      int checkRotate = i % travelPerRotation;  //Check if a rotation step must be made
      if (checkRotate == 0)
        digitalWrite(rotStepPin, HIGH);
      if (travTime > 330) {
        delay(interval / 2);
      } else {
        delayMicroseconds(interval / 2);
      }
      digitalWrite(travStepPin, LOW);
      if (checkRotate == 0)
        digitalWrite(rotStepPin, LOW);
      if (travTime > 330) {
        delay(interval / 2);
      } else {
        delayMicroseconds(interval / 2);
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

void displayLimitReached() {
  display.setCursor(28, 4);
  display.print(F("Limit Reached"));
  display.display();
}

void backOffMotor(int pin, int dirPin, float interval) {
  int travelDir = digitalRead(dirPin);
  digitalWrite(dirPin, !travelDir);
  for (int j = 1; j < 100; j++) {
    pulseMotor(pin, interval);
  }
  digitalWrite(dirPin, travelDir);
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
  inputField(0, 0, 1, 1);
  dataInputNo = 1;
  inputField(0, minTravDist, maxTravDist, travDistInc);
  dataInputNo = 2;
  inputField(0, 0, 1, 1);
  dataInputNo = 3;
  inputField(initialRotAng, minRotAng, maxRotAng, rotAngInc);
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
            //Serial.println("Input Confirmed");
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
void updateDistanceRotationDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(2, 2);
  display.print(F("Trav. Dir: "));
  display.setCursor(2, 12);
  display.print(F("Distance: "));
  display.setCursor(2, 22);
  display.print(F("Rot. Dir: "));
  display.setCursor(2, 32);
  display.print(F("Rot. Ang: "));


  int selected = 0;
  switch (dataInputNo) {
    case 0:
      selected = 2;
      travelDir = encoderPos;
      break;
    case 1:
      selected = 12;
      travDist = encoderPos;
      break;
    case 2:
      selected = 22;
      rotDir = encoderPos;
      break;
    default:
      selected = 32;
      rotAngle = encoderPos;
      break;
  }

  display.setCursor(65, selected);
  display.print(F(">"));
  display.setCursor(75, 2);
  display.print(travelDir == 0 ? F("Right") : F("Left"));
  display.setCursor(75, 12);
  display.print(travDist);
  display.print(F("mm"));
  display.setCursor(75, 22);
  display.print(rotDir == 0 ? F("CCW") : F("CW"));
  display.setCursor(75, 32);
  display.print(rotAngle);
  display.print(F("deg"));
  display.display();
}

// Function to handle the timing display
void updateTimingDisplay() {
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
    case 4:
      selected = 2;
      numHours = encoderPos;
      break;
    case 5:
      selected = 12;
      numMinutes = encoderPos;
      break;
    case 6:
      selected = 22;
      numSeconds = encoderPos;
      break;
    case 7:
      selected = 32;
      countDown = encoderPos;
      break;
    default:
      selected = 42;
      numLoops = encoderPos;
      break;
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
    updateDistanceRotationDisplay();
  }
  // Display timing parameters if dataInputNo is 4 or greater
  else {
    updateTimingDisplay();
  }
}

// Updated function for point A data display using the consolidated function
void updatePointADataDisplay() {
  // Same logic as updatePanAndRotateDataDisplay
  if (dataInputNo <= 3) {
    updateDistanceRotationDisplay();
  } else {
    updateTimingDisplay();
  }
}

int calcTravelPulses() {
  return travDist * pulsesPerMM;
}

int calcRotationPulses() {
  return rotAngle * pulsesPerDeg;
}

float calcInterval(int numPulses) {  //Calculate the interval required between pulses to achieve duration

  // NOTE:
  // Experimentally found that delay function works best for times greater than 330 seconds and microdelay works better for times less than 330 seconds
  // THIS WILL CHANGE WITH TRACK LENGTH
  float inter = 0.0;
  if (travTime > 330) {
    inter = travTime * 1000 / numPulses;
  } else {
    inter = travTime * 1000000 / numPulses;
  }
  return inter;
}

// TODO: Add flag so this isn't always moving slider in the background
void moveToPosition() {
  int pulses;
  float interval = 1000000.0 / defaultSpeed;  // Default speed in pulses per second

  switch (dataInputNo) {
    case 0:  // Travel direction
      digitalWrite(travDirPin, travelDir == 0 ? LOW : HIGH);
      break;
    case 1:  // Travel distance
      pulses = abs(travDist - prevTravDist) * pulsesPerMM;
      if (travDist < prevTravDist) {
        digitalWrite(travDirPin, HIGH);
      } else {
        digitalWrite(travDirPin, LOW);
      }
      moveMotor(pulses, interval, travStepPin, travDirPin, leftLimitSwitch, rightLimitSwitch);
      prevTravDist = travDist;
      break;
    case 2:  // Rotation direction
      digitalWrite(rotDirPin, rotDir == 0 ? HIGH : LOW);
      break;
    case 3:  // Rotation angle
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