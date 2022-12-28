// Camera Slider Version 2
// By: Todd Spencer
// Updates:
//  - Updated Logo
//  - Decreased max pan to avoid overruns
//  - Max duration is 5 minutes and 30 seconds due to pulse width limitations
//  - Made the mininum travel distance and rotation both 0, and got rid of the Pan Only 
//    or Rotate Only options, user can just set either pan/rotate to null if they want to 
//    just do one or the other

#include <SPI.h>                            //Import libraries to control the OLED display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>

#define SCREEN_WIDTH 128                    // OLED display width, in pixels
#define SCREEN_HEIGHT 64                    // OLED display height, in pixels

#define OLED_RESET -1                                                           // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);       // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)

// 'Astronaut Logo', 128x64px
const unsigned char logo [] PROGMEM = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
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
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};


#define minTravDist 0                       //Define travel distance initial setting, minimum, maximum and increment
#define maxTravDist 450                     //Changed from 550mm to 450mm to avoid overruns
#define travDistInc 25
#define initialDur 120                      //Define duration initial setting, minimum, maximum and increment
#define minDur 10
#define maxDur 330                          // Max allowable duration is 5min 30 seconds due to pulse size
#define durInc 5
#define initialRotAng 180                   //Define rotation initial setting, minimum, maximum and increment
#define minRotAng 0
#define maxRotAng 360
#define rotAngInc 10
#define initialObjDist 200                  //Define object distance initial setting, minimum, maximum and increment
#define minObjDist 150
#define maxObjDist 5000
#define objInc 50
#define minInterval 400                     //Minimum interval time between pulses in microseconds
#define minCountDown 5                      //Define minimum, maximum, and increment for countdown variable 
#define maxCountDown 120
#define countDownInc 5

static int pinA = 2;                        //Hardware interrupt digital pin 2
static int pinB = 3;                        //Hardware interrupt digital pin 3
volatile byte aFlag = 0;                    //Rising edge on pinA to signal that the encoder has arrived at a detent
volatile byte bFlag = 0;                    //Rising edge on pinB to signal that the encoder has arrived at a detent (opposite direction to when aFlag is set)
volatile int encoderPos = 0;                //Current value of encoder position, constained between limits below
volatile int prevEncoderPos = 0;            //To track whether the encoder has been turned and the display needs to update
volatile byte reading = 0;                  //Stores direct value from interrupt pin
#define encButton 4                         //Define encoder pushbutton pin
byte oldButtonState = HIGH;                 //First button state is open because of pull-up resistor
const unsigned long debounceTime = 10;      //Debounce delay time
unsigned long buttonPressTime;              //Time button has been pressed for debounce

int encLowLim = 0;                          //Variables to store the encoder limits and increment
int encHighLim = 3;
int encIncrement = 1;
int dataInputNo = 0;
int modeSelected = 0;                       //Current operating mode (Pan & Rotate, Track Object)

#define enablePin 5                         //Define motor enable pin
#define travDirPin 6                        //Define travel & rotation stepper motor pins
#define travStepPin 7
#define rotDirPin 8
#define rotStepPin 9

float travDist = maxTravDist;               //Distance to travel across slider in millimeters
float travTime = initialDur;                //Travel time to cover the required distance in seconds
float objDist = initialObjDist;             //Distance of tracked object from slider in millimeters
int travelDir = 0;                          //Deifne initial travel and rotation directions
int rotDir = 0;
int rotAngle = 180;                         //Angle to rotate camera around axis
int countDown = minCountDown;

float pulsesPerMM = 50;                     //Number of motor pulses for 1mm travel
float pulsesPerDeg = 4.4444;                //Number of motor pulses for 1 degree of rotation
float currentDist = 0;
float currentAngle = 0;

void setup() 
{
  Serial.begin(9600);                                     //Start serial communication for debugging
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))          //Connect to the OLED display
  {
    Serial.println(F("SSD1306 allocation failed"));       //If connection fails
    for(;;);                                              //Don't proceed, loop forever
  }
  pinMode(pinA, INPUT_PULLUP);                            //Set pinA as an input, pulled HIGH to the logic voltage
  pinMode(pinB, INPUT_PULLUP);                            //Set pinB as an input, pulled HIGH to the logic voltage
  attachInterrupt(0,PinA,RISING);                         //Set an interrupt on PinA
  attachInterrupt(1,PinB,RISING);                         //Set an interrupt on PinB
  pinMode (encButton, INPUT_PULLUP);                      //Set the encoder button as an input, pulled HIGH to the logic voltage
  pinMode(enablePin, INPUT);                              //Open circuit enable pin, disables motors
  pinMode(travDirPin, OUTPUT);                            //Define the travel stepper motor pins
  pinMode(travStepPin, OUTPUT);
  pinMode(rotDirPin, OUTPUT);                             //Define the rotation stepper motor pins
  pinMode(rotStepPin, OUTPUT);
  digitalWrite(travDirPin, HIGH);                         //Set the initial direction of motion for both motors
  digitalWrite(rotDirPin, HIGH);
  display.clearDisplay();                                 //Clear the display
  display.setTextColor(SSD1306_WHITE);                    //Set the text colour to white
  display.drawBitmap(0, 0, logo, 128, 64, WHITE);         //Display bitmap from array
  display.display();
  delay(2000);
  display.clearDisplay();                                 //Clear display
  Serial.println("Setup complete");                       //Write to serial monitor to indicate the setup function is complete
}

void loop() 
{  
  encLowLim = 0;                                            //Mode selection menu, 4 modes
  encHighLim = 3;
  encIncrement = 1;
  updateMainMenu();
  
  boolean confirmed = false;                                //Both used to confirm button push to select mode
  boolean pressed = false;
  encoderPos = 0;                                           //Encoder starts from 0, first menu option
  while(!confirmed)                                         //While the user has not confirmed the selection
  {
    byte buttonState = digitalRead (encButton); 
    if (buttonState != oldButtonState)
    {
      if (millis () - buttonPressTime >= debounceTime)      //Debounce button
      {
        buttonPressTime = millis ();                        //Time when button was pushed
        oldButtonState =  buttonState;                      //Remember button state for next time
        if (buttonState == LOW)
        {
          modeSelected = encoderPos;                        //If the button is pressed, accept the current digit into the guessed code
          pressed = true;
          Serial.println("Button Pushed");
        }
        else 
        {
          if (pressed == true)                              //Confirm the input once the button is released again
          {
            confirmed = true;
            Serial.println("Mode confirmed");
          }
        }  
      }
    }
    if(encoderPos!=prevEncoderPos)                          //Update the display if the encoder position has changed
    {
      updateMainMenu();
      prevEncoderPos=encoderPos;
    }
  }
  Serial.println("Mode selected");
  if (modeSelected == 0)                                    //Run required mode function depending on selection
    runPanAndRotate ();
  else
    runTrack ();
}

void PinA()                                             //Rotary encoder interrupt service routine for one encoder pin
{
  cli();                                                //Stop interrupts happening before we read pin values
  reading = PIND & 0xC;                                 //Read all eight pin values then strip away all but pinA and pinB's values
  if(reading == B00001100 && aFlag)                     //Check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
  {     
    if(encoderPos<=(encHighLim-encIncrement))
      encoderPos = encoderPos+encIncrement;             //Increment the encoder's position count , only when below upper limit
    else
      encoderPos = encHighLim;                          //Stop at maximum, being upper limit
    bFlag = 0;                                          //Reset flags for the next turn
    aFlag = 0;                                          //Reset flags for the next turn
  }
  else if (reading == B00000100)                        //Signal that we're expecting pinB to signal the transition to detent from free rotation
    bFlag = 1;
  sei();                                                //Restart interrupts
}

void PinB()                                             //Rotary encoder interrupt service routine for the other encoder pin
{
  cli();                                                //Stop interrupts happening before we read pin values
  reading = PIND & 0xC;                                 //Read all eight pin values then strip away all but pinA and pinB's values
  if (reading == B00001100 && bFlag)                    //Check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
  {
    if(encoderPos>=(encLowLim+encIncrement))
      encoderPos = encoderPos-encIncrement;             //Decrement the encoder's position count, only when above lower limit
    else
      encoderPos = encLowLim;                           //Stop at minimum, being lower limit
    bFlag = 0;                                          //Reset flags for the next turn
    aFlag = 0;                                          //Reset flags for the next turn
  }
  else if (reading == B00001000)                        //Signal that we're expecting pinA to signal the transition to detent from free rotation
    aFlag = 1;
  sei();                                                //Restart interrupts
}

void updateMainMenu()                               //Updates the display data for the main menu
{
  display.clearDisplay();                           //Clear display
  display.setTextSize(1);                           //Set the text size
  display.setCursor(28,4);
  display.print(F("Camera Slider"));
  display.setCursor(25,20);                         //Set the display cursor position
  display.print(F("Pan & Rotate"));                 //Set the display text
  display.setCursor(25,30);
  display.print(F("Track Object"));
  
  int selected = 0;                                 //Stores cursor vertical position to show selected item
  if (encoderPos == 0)
    selected = 20;
  else
    selected = 30;
  display.setCursor(14,selected);                    //Set the display cursor position
  display.print(F(">"));
  display.display();                                //Output the display text
}

void runPanAndRotate()  //Runs the pan and rotate mode sequence
{
  inputPanAndRotateData();  //Get user inputs for pan movement
  displayStart();           //Display startup sequence and enable motors
  display.setCursor(30, 30);
  display.print(F("Pan & Rotate"));
  display.display();
  if (travelDir == 0)  //Set motor travel direction
    digitalWrite(travDirPin, LOW);
  else
    digitalWrite(travDirPin, HIGH);
  if (rotDir == 0)  //Set motor travel direction
    digitalWrite(rotDirPin, HIGH);
  else
    digitalWrite(rotDirPin, LOW);

  int travelPulses = calcTravelPulses();  //Calculate the number of motor pulses required to move the travel distance
  Serial.print("Travel pulses: ");
  Serial.println(travelPulses);
  int rotationPulses = calcRotationPulses();  //Calculate the number of motor pulses required to rotate the required angle
  Serial.print("Rotation pulses: ");
  Serial.println(rotationPulses);
  Serial.print("Travel Time: ");
  Serial.println(travTime);

// Enter this code if you just want to pan
if (travelPulses != 0 && rotationPulses == 0) {
  float interval = calcInterval(travelPulses);  //Calculate the pulse interval required to move the required distance in the required time
  Serial.print("Interval: ");
  Serial.println(interval);
  for (int i = 1; i <= travelPulses; i++)  //Pulse the motor to move the required distance in the required time
  {
    digitalWrite(travStepPin, HIGH);
    delayMicroseconds(interval / 2);
    digitalWrite(travStepPin, LOW);
    delayMicroseconds(interval / 2);
  }
}

// Enter this code if you just want to rotate
else if (travelPulses == 0 && rotationPulses != 0) {
  float interval = calcRotInterval(rotationPulses);  //Calculate the pulse interval required to rotate in the required time
  Serial.print("Interval: ");
  Serial.println(interval);
  for (int i = 1; i <= rotationPulses; i++)  //Pulse the motor to rotate the required angle in the required time
  {
    digitalWrite(rotStepPin, HIGH);
    delay(interval / 2);
    digitalWrite(rotStepPin, LOW);
    delay(interval / 2);
  }
}

  // Enter this code if you have non-zero pan and rotation inputs
  else {
    float interval = calcInterval(travelPulses);  //Calculate the pulse interval required to move the required distance in the required time
    Serial.print("Interval: ");
    Serial.println(interval);
    int travelPerRotation = travelPulses / rotationPulses;  //Calculate how much the camera should pan for each rotation step
    for (int i = 1; i <= travelPulses; i++) {
      digitalWrite(travStepPin, HIGH);
      int checkRotate = i % travelPerRotation;  //Check if a rotation step must be made
      if (checkRotate == 0)
        digitalWrite(rotStepPin, HIGH);
      delayMicroseconds(interval / 2);
      digitalWrite(travStepPin, LOW);
      if (checkRotate == 0)
        digitalWrite(rotStepPin, LOW);
      delayMicroseconds(interval / 2);
      /*currentDist = i/pulsesPerMM;
      currentAngle = i/pulsesPerDeg;
      Serial.print("Dist: ");
      Serial.println(currentDist);
      Serial.print("Angle: ");
      Serial.println(currentAngle);*/
    }
  }

  displayEnd();  //Display the end sequence and disable motors
}

void runTrack ()                                                //Runs the object tracking mode sequence
{
  inputTrackData ();                                             //Get user inputs for tracking movement
  displayStart ();
  display.setCursor(22,30);
  display.print(F("Object Tracking"));
  display.display();
  if (travelDir == 0)                                             //Set motor travel and rotate directions
  {
    digitalWrite(travDirPin, LOW);
    digitalWrite(rotDirPin, LOW);
  }
  else
  {
    digitalWrite(travDirPin, HIGH);
    digitalWrite(rotDirPin, HIGH);
  }
  int travelPulses = calcTravelPulses ();                       //Calculates the number of pulses required to move the travel distance
  Serial.print("Travel pulses: ");
  Serial.println(travelPulses);
  float interval = calcInterval (travelPulses);             //Runs through movement sequence to move m    //Calculates the interval required to achieve the required duration
  Serial.print("Interval: ");
  Serial.println(interval);
  currentAngle = atan((objDist)/(travDist/2))*180/M_PI;         //Calculates the initial camera to object angle
  Serial.print("Current Angle: ");
  Serial.println(currentAngle);
  for (int i=1; i<=(travelPulses/2); i++)                         
  {
      digitalWrite(travStepPin, HIGH);
      boolean rotatePulse = checkRot (i);
      if (rotatePulse == true)
        digitalWrite(rotStepPin, HIGH);
      delayMicroseconds(interval/2);
      digitalWrite(travStepPin, LOW);
      if (rotatePulse == true)
        digitalWrite(rotStepPin, LOW);
      delayMicroseconds(interval/2);
      currentDist = i/pulsesPerMM;
      /*Serial.print("Dist: ");
      Serial.println(currentDist);
      Serial.print("Angle: ");
      Serial.println(currentAngle);*/
  }
  currentAngle = 90;
  for (int i=((travelPulses/2)+1); i<=travelPulses; i++)                           //Runs through movement sequence to move motors
  {
      digitalWrite(travStepPin, HIGH);
      boolean rotatePulse = checkRot (i);
      if (rotatePulse == true)
        digitalWrite(rotStepPin, HIGH);
      delayMicroseconds(interval/2);
      digitalWrite(travStepPin, LOW);
      if (rotatePulse == true)
        digitalWrite(rotStepPin, LOW);
      delayMicroseconds(interval/2);
      currentDist = i/pulsesPerMM;
      /*Serial.print("Dist: ");
      Serial.println(currentDist);
      Serial.print("Angle: ");
      Serial.println(currentAngle);*/
  }
  displayEnd();                                           //Display the end sequence and disable motors
}

void displayStart()
{
  display.clearDisplay();                                   //Clear display
  display.setTextSize(1);                                   //Set the text size
  display.setCursor(45,20);                                 //Set the display cursor position
  display.print(F("Push To"));                              //Set the display text
  display.setCursor(50,32);
  display.print(F("Start"));
  display.display();                                        //Output the display text
  boolean confirmed = false;                                //Both used to confirm button push to select mode
  boolean pressed = false;
  while(!confirmed)                                         //While the user has not started the panning routine
  {
    byte buttonState = digitalRead (encButton); 
    if (buttonState != oldButtonState)
    {
      if (millis () - buttonPressTime >= debounceTime)      //Debounce button
      {
        buttonPressTime = millis ();                        //Time when button was pushed
        oldButtonState =  buttonState;                      //Remember button state for next time
        if (buttonState == LOW)
        {
          pressed = true;
        }
        else 
        {
          if (pressed == true)                              //Confirm the input once the button is released again
          {
            confirmed = true;
          }
        }  
      }
    }
  }
  pinMode(enablePin, OUTPUT);                               //Enable the motors
  for(int i=countDown ; i> 0 ; i--)                         //Countdown to start
  {
    display.clearDisplay();                                 //Clear display
    display.setTextSize(2);                                 //Set the text size
    display.setCursor(60,20);                               //Set the display cursor position
    display.print(i);                                       //Set the display text
    display.display(); 
    delay(1000);
  }
  display.clearDisplay();                                   //Clear display
  display.setTextSize(1);                                   //Set the text size
  display.setCursor(45,15);                                 //Set the display cursor position
  display.print(F("Running"));                              //Set the display text
  display.display();
}

void displayEnd()
{
  display.clearDisplay();                                   //Clear display
  display.setTextSize(1);                                   //Set the text size
  display.setCursor(40,15);                                 //Set the display cursor position
  display.print(F("Complete"));                             //Set the display text
  display.setCursor(44,35);
  display.print(F("Push To"));
  display.setCursor(27,47);
  display.print(F("Release Motors"));
  display.display();                                        //Output the display text
  boolean confirmed = false;                                //Both used to confirm button push to select mode
  boolean pressed = false;
  while(!confirmed)                                         //While the user has not started the routine
  {
    byte buttonState = digitalRead (encButton); 
    if (buttonState != oldButtonState)
    {
      if (millis () - buttonPressTime >= debounceTime)      //Debounce button
      {
        buttonPressTime = millis ();                        //Time when button was pushed
        oldButtonState =  buttonState;                      //Remember button state for next time
        if (buttonState == LOW)
        {
          pressed = true;
        }
        else 
        {
          if (pressed == true)                              //Confirm the input once the button is released again
          {
            confirmed = true;
          }
        }  
      }
    }
  }
  pinMode(enablePin, INPUT);                                //Open circuit enable pin, disables motors
  resetVariables ();
}

void inputPanAndRotateData ()                                                     //Input required data for pan and rotate mode
{
  dataInputNo = 0;                                                                //Input pan distance
  inputField (maxTravDist, minTravDist, maxTravDist, travDistInc);
  dataInputNo = 1;                                                                //Input pan direction
  inputField (0, 0, 1, 1);
  dataInputNo = 2;                                                                //Input rotation angle
  inputField (initialRotAng, minRotAng, maxRotAng, rotAngInc);
  dataInputNo = 3;                                                                //Input rotation direction
  inputField (0, 0, 1, 1);
  dataInputNo = 4;                                                                //Input total duration
  inputField (initialDur, minDur, maxDur, durInc);
  dataInputNo = 5;                                                                //Input count down
  inputField (minCountDown, minCountDown, maxCountDown, countDownInc);
}

void inputTrackData ()                                                            //Input required data for object tracking mode
{
  dataInputNo = 0;                                                                //Input pan distance
  inputField (maxTravDist, minTravDist, maxTravDist, travDistInc);
  dataInputNo = 1;                                                                //Input pan direction
  inputField (0, 0, 1, 1);
  dataInputNo = 2;                                                                //Input rotation angle
  inputField (initialObjDist, minObjDist, maxObjDist, objInc);
  dataInputNo = 3;                                                                //Input total duration
  inputField (initialDur, minDur, maxDur, durInc);
  dataInputNo = 4;                                                                //Input count down
  inputField (minCountDown, minCountDown, maxCountDown, countDownInc);
}

void updatePanAndRotateDataDisplay ()
{
  display.clearDisplay();                                   //Clear display
  display.setTextSize(1);                                   //Set the text size
  display.setCursor(2,2);                                   //Set the display cursor position
  display.print(F("Distance: "));                           //Set the display text
  display.setCursor(2,12);
  display.print(F("Trav. Dir: "));
  display.setCursor(2,22);
  display.print(F("Rot. Ang: "));
  display.setCursor(2,32);
  display.print(F("Rot. Dir: "));
  display.setCursor(2,42);
  display.print(F("Duration: "));
  display.setCursor(2,52);
  display.print(F("Count Down: "));
  int selected = 0;
  if (dataInputNo == 0)                                     //Get the cursor position & update changing variable
  {
    selected = 2;
    travDist = encoderPos;
  }
  else if (dataInputNo == 1)
  {
    selected = 12;
    travelDir = encoderPos;
  }
  else if (dataInputNo == 2)
  {
    selected = 22;
    rotAngle = encoderPos;
  }
  else if (dataInputNo == 3)
  {
    selected = 32;
    rotDir = encoderPos;
  }
  else if (dataInputNo == 4)
  {
    selected = 42;
    travTime = encoderPos;
    if(calcInterval (calcTravelPulses ()) < minInterval)    //Flags movement too fast
    {
      display.setCursor(40,55);                             //Set the display cursor position
      display.print(F("Too Fast"));                         //Set the display text
    }
  }
  else
  {
    selected = 52;
    countDown = encoderPos;
  }
  display.setCursor(65,selected);                           //Set the display cursor position
  display.print(F(">"));
  display.setCursor(75,2);                                  //Display the field data
  display.print(travDist);
  display.print(F("mm"));
  display.setCursor(75,12);
  if (travelDir == 0)
    display.print(F("Forward"));
  else
    display.print(F("Reverse"));
  display.setCursor(75,22);
  display.print(rotAngle);
  display.print(F("deg"));
  display.setCursor(75,32);
  if (rotDir == 0)
    display.print(F("Forward"));
  else
    display.print(F("Reverse"));
  display.setCursor(75,42);
  display.print(travTime);
  display.print(F("s"));
  display.setCursor(75,52);
  display.print(countDown);
  display.print(F("s"));
  display.display();                                        //Output the display text
}

void updateTrackDataDisplay ()
{
  display.clearDisplay();                                   //Clear display
  display.setTextSize(1);                                   //Set the text size
  display.setCursor(2,10);                                  //Set the display cursor position
  display.print(F("Distance: "));                           //Set the display text
  display.setCursor(2,20);
  display.print(F("Trav. Dir: "));
  display.setCursor(2,30);
  display.print(F("Obj. Dist: "));
  display.setCursor(2,40);
  display.print(F("Duration: "));
  display.setCursor(2,50);
  display.print(F("Count Down: "));
  int selected = 0;
  if (dataInputNo == 0)                                     //Get the cursor position & update changing variable
  {
    selected = 10;
    travDist = encoderPos;
  }
  else if (dataInputNo == 1)
  {
    selected = 20;
    travelDir = encoderPos;
  }
  else if (dataInputNo == 2)
  {
    selected = 30;
    objDist = encoderPos;
  }
  else if (dataInputNo == 3)
  {
    selected = 40;
    travTime = encoderPos;
    if(calcInterval (calcTravelPulses ()) < minInterval)    //Flags movement too fast
    {
      display.setCursor(40,55);                             //Set the display cursor position
      display.print(F("Too Fast"));                         //Set the display text
    }
  }
  else
  {
    selected = 50;
    countDown = encoderPos;
  }
  display.setCursor(65,selected);                           //Set the display cursor position
  display.print(F(">"));
  display.setCursor(75,10);                                 //Display the field data
  display.print(travDist);
  display.print(F("mm"));
  display.setCursor(75,20);
  if (travelDir == 0)
    display.print(F("Forward"));
  else
    display.print(F("Reverse"));
  display.setCursor(75,30);
  display.print(objDist);
  display.print(F("mm"));
  display.setCursor(75,40);
  display.print(travTime);
  display.print(F("s"));
  display.setCursor(75,50);
  display.print(countDown);
  display.print(F("s"));
  display.display();                                        //Output the display text
}

void inputField (int initialSetting, int lowerLimit, int upperLimit, int increment)
{
  encLowLim = lowerLimit;
  encHighLim = upperLimit;
  encIncrement = increment;
  encoderPos = initialSetting;                              //Encoder starts from initial setting
  prevEncoderPos = encoderPos+1;                            //Set different so that display is updated on first cycle
  boolean confirmed = false;                                //Both used to confirm button push to select mode
  boolean pressed = false;
  while(!confirmed)                                         //While the user has not confirmed the input
  {
    byte buttonState = digitalRead (encButton); 
    if (buttonState != oldButtonState)
    {
      if (millis () - buttonPressTime >= debounceTime)      //Debounce button
      {
        buttonPressTime = millis ();                        //Time when button was pushed
        oldButtonState =  buttonState;                      //Remember button state for next time
        if (buttonState == LOW)
        {
          pressed = true;
        }
        else 
        {
          if (pressed == true)                              //Confirm the input once the button is released again
          {
            confirmed = true;
            Serial.println("Input Confirmed");
          }
        }  
      }
    }
    if(encoderPos!=prevEncoderPos)                          //Update the display if the encoder position has changed
    {
      if (modeSelected == 0)
        updatePanAndRotateDataDisplay ();
      else
        updateTrackDataDisplay ();
      prevEncoderPos=encoderPos;
    }
  }
}

void resetVariables ()                                       //Reset variables back to initial values after run
{
  travDist = maxTravDist;
  travTime = initialDur;
  objDist = initialObjDist;
  travelDir = 0;
  rotDir = 0;
  rotAngle = initialRotAng;
  countDown = minCountDown;
}

int calcTravelPulses ()                                       //Calculates the number of pulses required to move a certain distance
{
  int travP = travDist*pulsesPerMM;
  return travP;
}

int calcRotationPulses ()                                     //Calculate the number of pulses required to rotate a certain angle
{
  int rotP = rotAngle*pulsesPerDeg;
  return rotP;
}

boolean checkRot (int i)                                      //Used in tracking to calculate the angle required to track object
{
  boolean rotP = false;
  float deltaAngle;
  if(((travDist/2)-(i/pulsesPerMM)) > 0)
  {
    if (currentAngle < 90-(1/pulsesPerDeg))
    {
      float newAngle = atan((objDist)/((travDist/2)-(i/pulsesPerMM)))*180/M_PI;
      deltaAngle = newAngle-currentAngle;
    }
  }
  else if (((travDist/2)-(i/pulsesPerMM)) < 0)
  {
    if (currentAngle > 0)
    {
      float newAngle = atan((objDist)/((i/pulsesPerMM)-(travDist/2)))*180/M_PI;
      deltaAngle = currentAngle-newAngle;
    }
  }
  if(deltaAngle >= (1/pulsesPerDeg))
  {
    rotP = true;
    if ((travDist/2)-(i/pulsesPerMM) > 0)
      currentAngle=currentAngle+(1/pulsesPerDeg);
    else
      currentAngle=currentAngle-(1/pulsesPerDeg);
  }
  return rotP;
}

float calcInterval (int numPulses)                                 //Calculate the interval required between pulses to achieve duration
{
  float inter = travTime*1000000/numPulses;
  return inter;
}

float calcRotInterval (int numPulses)                              //Calculate the interval required between pulses to achieve duration
{
  float inter = travTime*1000/numPulses;
  return inter;
}
