#include "AutoPilot.h"

AutoPilot::AutoPilot(int switchPin, int pullupPin):
actionTimer()
{
  Switches = switchPin;
  SwitchPullup = pullupPin;
  actionTimer.start();
  setPins();

  backingUp = false;
}

void AutoPilot::setPins() {
  pinMode(SwitchPullup, OUTPUT);
  digitalWrite(SwitchPullup, HIGH);
  pinMode(Switches, INPUT);
}

bool AutoPilot::collisionDetected(){
 int switchvalue = analogRead(Switches);

 return (switchvalue < 500);
}

void AutoPilot::controlCar(Car& car){
  bool crashDetected = collisionDetected();
  if((crashDetected && !backingUp) || actionTimer.isFinished()){
    startNextAction(car, crashDetected);
  }
}

void AutoPilot::startNextAction(Car& car, bool crashDetected){
 if(crashDetected){
  Serial.println("Backing up");
  backingUp = true;
   car.moveBackward();
   actionDelay(500);
 }
 else if(backingUp){
  Serial.println("turning right");
  backingUp = false;
  turnToRandomDirection(car);
  car.turnRight();
  actionDelay(500);
 }
 else if(!crashDetected && !backingUp){
  Serial.println("Move forward");
  car.moveForward();
  actionDelay(5000);
 }
}

 void AutoPilot::turnToRandomDirection(Car& car){
  if(millis() % 2){
    car.turnRight();
  }
  else{
    car.turnLeft();
  }
 }
 
void AutoPilot::actionDelay(int duration){
  actionTimer.setInterval(duration);
  actionTimer.reset();
}

