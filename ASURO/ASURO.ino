#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
#include "Car.h"
#include "LineFollower.h"
#include "AutoPilot.h"

//Pin Definitions
//Motor Controllers
const int LeftSpeed = 5;
const int LeftForward = 7;
const int LeftBackward = 8;

const int RightSpeed = 6;
const int RightForward = 9;
const int RightBackward = 10;

//Speedometer
const int OdometerLED = 12;
const int LeftOdoSensor = A6;
const int RightOdoSensor = A7;

//Switches
const int Switches = A0;
const int SwitchPullup = 11;

//Line Follower
const int LineFollowerLED = A1;
const int LeftLFSensor = A2;
const int RightLFSensor = A3;

#define SERIAL_DEBUG
#define OLED_DEBUG
const int DebugLED = 13;

//Objects
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
Car car(LeftSpeed, LeftForward, LeftBackward, RightSpeed, RightForward, RightBackward);
LineFollower lineFollower(LineFollowerLED, LeftLFSensor, RightLFSensor, 10);
AutoPilot autoPilot(Switches, SwitchPullup);

Timer signalTimeout(200);
SoftwareSerial btModule(3, 2); // RX, TX

//Global Variables
enum modes {MANUAL_OPERATION,LINE_FOLLOWING,OBSTACLE_AVOIDING};
int mode = MANUAL_OPERATION;
bool crashed = false;
bool commandAvailable = false;
bool requestAvailable = false;
String command = "";

//Function Prototypes
void sendDashboardData(void);
void controlSystems(String input);
void controlCar(String input);
void setAutopilot(String input);
void controlLights(String input);

char direction = 'S';

void setup() {
  btModule.begin(9600);
  Serial.begin(9600);
  signalTimeout.start();

#ifdef DEBUG1
  startDebugSession();
#endif

//lineFollower.calibrateSensors();
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
display.clearDisplay();

display.setTextColor(WHITE);
display.setCursor(0,0);
display.setTextSize(1);
 display.println("Hello, world!");
 display.display();
 delay(2000);
}

void loop() {
  crashed = autoPilot.collisionDetected();

if(btModule.available())
{
  String input = btModule.readStringUntil('X');
  if(input == "status?")
  {
    sendDashboardData();
  }
  else
  {
    printCommand();
    command = input;   
    controlSystems(command);
  }
  signalTimeout.reset();
}
updateCar();

car.update();
}

void printCommand(void)
{
  static String previousCommand = "";
  if(previousCommand != command)
  {
    previousCommand = command;
  #ifdef SERIAL_DEBUG
  Serial.print("Latest Command: ");
  Serial.println(command);
  #endif
  #ifdef OLED_DEBUG
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Latest Command: ");
  display.println(command);
  display.display();
  #endif
  }
}

void updateCar(void)
{
   switch (mode) {
    case MANUAL_OPERATION: 
      if (crashed && direction == 'F')
      {
        display.clearDisplay();
        display.setCursor(0,0);
        display.println("Crash!");
         display.display();
        car.stop();
      }
      if (signalTimeout.isFinished()) 
      {
        car.stop();
        direction = 'S';
        signalTimeout.stop();
      }
      break;
    case LINE_FOLLOWING: 
      if (crashed) 
      {
        mode = MANUAL_OPERATION;
        car.stop();
        break;
      }
      signalTimeout.stop();
      lineFollower.controlCar(car);
      break;
    case OBSTACLE_AVOIDING: signalTimeout.stop();
      autoPilot.controlCar(car);
      break;
    default: break;
  }
}

void controlSystems(String input) {
  char commandType = input.charAt(0);
  input.remove(0, 1);

  switch (commandType) {
    case'C': controlCar(input);
      break;
    case 'A': setAutopilot(input);
      break;
    case 'L': controlLights(input);
    break;
    case'T': setSpeed(input);
      break;
    default: break;
  }
}

 void sendDashboardData(void)
 {
String testSpeed = String( ( (millis()%6) + 1 ) + ( ( millis()%10 ) * 0.1 ) );
String testVoltage = "1.1";
String testLowVoltageFlag = toStateMessage(false);
String lineFollowingState = toStateMessage(mode == LINE_FOLLOWING);
String obstacleAvoidingState = toStateMessage(mode == OBSTACLE_AVOIDING);
String lightState = toStateMessage(digitalRead(DebugLED));

  int numberOfItems = 6;
  String items[numberOfItems] = {testSpeed,testVoltage,testLowVoltageFlag,lineFollowingState,obstacleAvoidingState,lightState};

int lastItemIndex = numberOfItems - 1;
  for(int i = 0; i < numberOfItems; i++)
  {
    btModule.print(items[i]);
    if(i < lastItemIndex)
    {
    btModule.print("|");
    }
  }
   btModule.println();
 
 }

 String toStateMessage(bool state)
{
  if(state)
  {
   return "On"; 
  }
  else
  {
    return "Off"; 
  }
}

void setSpeed(String input){
int value = input.toInt();
  if(value >= 0 && value <= 100){
    car.setMaxSpeed(value);
  }
}

void controlCar(String input) {

  char firstDirectionLetter = input.charAt(0);
  input.remove(0, 1);
  char secondDirectionLetter = 0;

  switch (firstDirectionLetter) {
    case 'S': car.stop();
      mode = 0;
      break;
    case 'F': if (!crashed) {
        direction = 'F';
        car.moveForward();
      }
      mode = 0;
      break;
    case 'B': car.moveBackward();
      mode = 0;
      break;
    case 'L': secondDirectionLetter = input.charAt(0);
      switch (secondDirectionLetter) {
        case 'T': car.turnLeft();
          break;
        case 'F': if (!crashed) {
            direction = 'F';
            car.leftForward();
          }
          break;
        case 'B': car.leftBackward();
          break;
        default: break;
      }
      mode = 0;
      break;

    case 'R':
      secondDirectionLetter = input.charAt(0);
      switch (secondDirectionLetter) {
        case 'T': car.turnRight();
          break;
        case 'F': if (!crashed) {
          direction = 'F';
            car.rightForward();
          }
          break;
        case 'B': car.rightBackward();
          break;
        default: break;
      }
      mode = 0;
      break;

    default: break;
  }
  signalTimeout.start();
}

void setAutopilot(String input) {
  char modeBuffer = input.charAt(0);
#ifdef DEBUG
  Serial.println("setting Autopilot");
  Serial.println(modeBuffer);
#endif

  switch (modeBuffer) {
    case '0': mode = MANUAL_OPERATION;
      car.stop();
      break;
    case '1': mode = LINE_FOLLOWING;
       car.setMaxSpeed(65);
      lineFollower.reset();
      car.stop();
      break;
    case '2': mode = OBSTACLE_AVOIDING;
      car.stop();
      break;
    default: break;
  }
}

void controlLights(String input) {
  char state = input.charAt(0);
  if (state == '1') {
    digitalWrite(DebugLED, HIGH);
  }
  else {
    digitalWrite(DebugLED, LOW);
  }
}


