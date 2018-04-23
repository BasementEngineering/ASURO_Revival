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
const int OdometerLED = 3;
const int LeftOdoSensor = A3;
const int RightOdoSensor = A4;

//Switches
const int Switches = A0;
const int SwitchPullup = 4;

//Line Follower
const int LineFollowerLED = 2;
const int LeftLFSensor = A1;
const int RightLFSensor = A2;

//#define DEBUG
//#define DEBUG1
const int DebugLED = 13;

//Objects
Car car(LeftSpeed, LeftForward, LeftBackward, RightSpeed, RightForward, RightBackward);
LineFollower lineFollower(LineFollowerLED, LeftLFSensor, RightLFSensor, 10);
AutoPilot autoPilot(Switches, SwitchPullup);
Timer signalTimeout(200);

//Global Variables
enum modes {MANUAL_OPERATION,LINE_FOLLOWING,OBSTACLE_AVOIDING};
int mode = MANUAL_OPERATION;
bool crashed = false;
bool commandAvailable = false;
bool requestAvailable = false;
String command = "";

//Function Prototypes
void startDebugSession();

void sendDashboardData(void);
void controlRobot(String input);
void controlCar(String input);
void setAutopilot(String input);
void controlLights(String input);

char direction = 'S';

void setup() {

  Serial.begin(9600);
  signalTimeout.start();

#ifdef DEBUG1
  startDebugSession();
#endif

//lineFollower.calibrateSensors();

}

void loop() {

  crashed = autoPilot.collisionDetected();

  if(requestAvailable)
  {
    sendDashboardData();
    requestAvailable = false;
  }

  if(commandAvailable)
  {
    controlRobot(command);
    commandAvailable = false;
  }
  
  switch (mode) {
    case MANUAL_OPERATION: if (crashed && direction == 'F'){
        car.stop();
      }
      if (signalTimeout.isFinished()) {
        car.stop();
        direction = 'S';
        signalTimeout.stop();
      }
      break;
    case LINE_FOLLOWING: if (crashed) {
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

void serialEvent() {
  String input = Serial.readStringUntil('X');
  if(input == "status?")
  {
    requestAvailable = true;
  }
  else
  {
  commandAvailable = true;
  command = input;
  }
}

void startDebugSession() {
  pinMode(DebugLED, OUTPUT);

  for (int i = 0; i < 3; i++) {
    digitalWrite(DebugLED, HIGH);
    delay(500);
    digitalWrite(DebugLED, LOW);
    delay(500);
  }

  Serial.println("Debugging enabled");
}

void controlRobot(String input) {
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

void toStateMessage(bool state)
{
  if(state)
  {
   Serial.print("On"); 
  }
  else
  {
    Serial.print("Off"); 
  }
}

 void sendDashboardData(void)
 {
  int numberOfItems = 6;

  //Speed
  Serial.print(1.1);
  Serial.print("|");
  //Voltage
  Serial.print(1.1);
  Serial.print("|");
  //Low Voltage Flag
  toStateMessage(false);
  Serial.print("|");
  //Line Following
  toStateMessage(mode == LINE_FOLLOWING);
  Serial.print("|");
  //Collision Detection
  toStateMessage(mode == OBSTACLE_AVOIDING);
  Serial.print("|");
  //Led state
  toStateMessage(digitalRead(DebugLED));

  Serial.println();
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

#ifdef DEBUG
  Serial.println("Received Contol Signal");
  Serial.println(firstDirectionLetter);
#endif

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


