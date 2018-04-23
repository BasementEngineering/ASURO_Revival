#include "LineFollower.h"
//#define DEBUG

LineFollower::LineFollower(int ledPin, int leftSensorPin, int rightSensorPin, int _tolerance):
  timeoutTimer(2000, false)
{
 tolerance = _tolerance;
  previousLineLocation = '0';
  
  LED = ledPin;
  LeftSensor = leftSensorPin;
  RightSensor = rightSensorPin;

  setPins();
  updateValues();
  
}

void LineFollower::reset(){
  previousLineLocation = '0';
  timeoutTimer.stop();
}

void LineFollower::setPins() {
  pinMode(LED, OUTPUT);
  pinMode(LeftSensor, INPUT);
  pinMode(RightSensor, INPUT);

  digitalWrite(LED, HIGH);
}

void LineFollower::calibrateSensors() {
  #ifdef DEBUG
  Serial.println("Calibrating Sensors");
  #endif
  blinkLED(5, 500);
  leftBlackValue = analogRead(LeftSensor);
  rightBlackValue = analogRead(RightSensor);
  #ifdef DEBUG
  Serial.println("Black Values");
  Serial.print("Left: ");
  Serial.println(leftBlackValue);
  Serial.print("Right: ");
  Serial.println(rightBlackValue);
  #endif
  delay(1000);
  blinkLED(5, 500);
  leftWhiteValue = analogRead(LeftSensor);
  rightWhiteValue = analogRead(RightSensor);
  #ifdef DEBUG
  Serial.println("White Values");
  Serial.print("Left: ");
  Serial.println(leftWhiteValue);
  Serial.print("Right: ");
  Serial.println(rightWhiteValue);
  #endif
}

void LineFollower::blinkLED(int times, int interval) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LED, LOW);
    delay(interval);
    digitalWrite(LED, HIGH);
    delay(interval);
  }
}

bool LineFollower::onLine(char side){
  int currentValue = 0;
  int setPoint = 0;

  switch (side){
 case 'L':
  currentValue = leftValue;
  setPoint = leftBlackValue;
  break;
 case 'R':
   currentValue = rightValue;
  setPoint = rightBlackValue;
  break;
 default: return false;
  }

return withinTolerance(currentValue, setPoint);

}

bool LineFollower::withinTolerance(int currentValue, int setPoint){
  return (currentValue > (setPoint - tolerance) && currentValue < (setPoint + tolerance));
}

void LineFollower::controlCar(Car& car) {
  #ifdef DEBUG
  Serial.println("Steering the car");
  #endif
  
  
  updateValues();
  
  if(!onLine('L') && onLine('R')){
    previousLineLocation = 'R';
    car.moveRight();
    timeoutTimer.stop();
    #ifdef DEBUG
  Serial.println("Move Right");
  #endif
  }
  else if(onLine('L') && !onLine('R')){
    previousLineLocation = 'L';
    car.moveLeft();
    timeoutTimer.stop();
     #ifdef DEBUG
  Serial.println("Move Left");
  #endif
  }

  if((onLine('L') == onLine('R')) && !(timeoutTimer.isActive())){
    timeoutTimer.start();
    
    #ifdef DEBUG
  Serial.println("Vehicle left the line");
  Serial.print("Previous line location was ");
  Serial.println(previousLineLocation);
  #endif
  
    switch(previousLineLocation){
      case 'R': car.moveRight();
      #ifdef DEBUG
  Serial.println("move Right");
  #endif
      break;
      case 'L': car.moveLeft();
      #ifdef DEBUG
  Serial.println("move Left");
  #endif
      break;
      case '0': car.moveForward();
      #ifdef DEBUG
  Serial.println("move Forward");
  #endif
      break;
      default: 
      break;
    }
    car.moveForward();
  }
   
  if(timeoutTimer.isFinished()){
    #ifdef DEBUG
  Serial.println("Vehicle could not find the line");
  #endif
    car.stop();
    previousLineLocation = '0';
  }

}

void LineFollower::updateValues() {
  leftValue = analogRead(LeftSensor);
  rightValue = analogRead(RightSensor);
  #ifdef DEBUG
  Serial.println("Latest Sensor Readings");
    Serial.print("Left: ");
  Serial.println(leftValue);
  Serial.print("Right: ");
  Serial.println(rightValue);
  #endif
}
