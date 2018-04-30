#include "Car.h"

Car::Car(int _leftSpeed, int _left1, int _left2, int _rightSpeed, int _right1, int _right2):
  actionTimer()
{
leftSpeed = _leftSpeed;
left1 = _left1;
left2 = _left2;

rightSpeed = _rightSpeed;
right1 = _right1;
right2 = _right2;

  pinMode(right1, OUTPUT);
  pinMode(right2, OUTPUT);
  pinMode(left1, OUTPUT);
  pinMode(left2, OUTPUT);
  pinMode(leftSpeed, OUTPUT);
  pinMode(rightSpeed, OUTPUT);

  direction = 0;
  currentSpeed = 0;
  directionCorrection = 0;
  minSpeed = 100;
  maxSpeed = 100;
  stop();
  carIsMoving = false;
}

bool Car::isMoving(){
  return carIsMoving;
}

void Car::startTimer(int duration) {
  actionTimer.setInterval(duration);
  actionTimer.start();
}

void Car::update() {
  if (carIsMoving) {
    if (actionTimer.isFinished()) {
      actionTimer.stop();
      stop();
    }
  }
}

void Car::setMaxSpeed(uint8_t newMaxSpeed){
  maxSpeed = newMaxSpeed;
}

void Car::setDirection(int newDirection) {
  if (newDirection >= -100 && newDirection <= 100) {
    direction = newDirection;
  }
}

void Car::setSpeed(int newSpeed) {
  if (newSpeed >= -100 && newSpeed <= 100) {
    currentSpeed = newSpeed;
  }
  else {
    currentSpeed = 0;
  }
}

void Car::setDirectionCorrection(int value) {
  if (value >= -100 && value <= 100) {
    directionCorrection = value;
  }
}

void Car::startMoving() {

  //removing the fixed minimum Speed
  int leftRange = 255 - minSpeed;
  int rightRange = 255 - minSpeed;

  //adding the direction Correction
  if (directionCorrection > 0) {
    rightRange = (float)((rightRange / 100) * (100 - directionCorrection));
  }
  else if (directionCorrection < 0) {
    leftRange = (float)((leftRange / 100) * (100 + directionCorrection));
  }

  //Adding the speed Parameter
  int rightSpeed = (float)(rightRange * currentSpeed / 100);
  int leftSpeed = (float)(leftRange * currentSpeed / 100) ;

  //Adding the direction
  if (direction < 0) {
    leftSpeed = (float)(rightSpeed * (100 + direction * 2) / 100) ;
  }
  else if (direction > 0) {
    rightSpeed = (float)(leftSpeed * (100 - direction * 2) / 100) ;
  }

#ifdef DEBUG
  Serial.print("Right Speed");
  Serial.println(rightSpeed);
  Serial.print("Left Speed");
  Serial.println(leftSpeed);
#endif

  //Translating it to the motors
  if (rightSpeed < 0) {
    rightBackward((uint8_t)((-rightSpeed) + minSpeed));
  }
  else if (rightSpeed > 0) {
    rightForward((uint8_t)(rightSpeed + minSpeed));
  }
  else {
    stopRight();
  }

  if (leftSpeed < 0) {
    leftBackward((uint8_t)(-leftSpeed + minSpeed));
  }
  else if (leftSpeed > 0) {
    leftForward((uint8_t)(leftSpeed + minSpeed));
  }
  else {
    stopLeft();
  }
}

void Car::moveForward(int duration)
{
  leftForward();
  rightForward();
  carIsMoving = true;

  if (duration) {
    startTimer(duration);
  }
#ifdef DEBUG
  Serial.println("go forward!");
#endif
}

void Car::moveBackward(int duration)
{
  leftBackward();
  rightBackward();
  carIsMoving = true;

  if (duration) {
    startTimer(duration);
  }
#ifdef DEBUG
  Serial.println("go backward!");
#endif
}

void Car::turnLeft(int duration)
{
  rightForward();
  leftBackward();

  if (duration) {
    startTimer(duration);
  }
#ifdef DEBUG
  Serial.println("go left fast!");
#endif
}

void Car::moveLeft(int duration)
{
  rightForward();

  if (duration) {
    startTimer(duration);
  }
#ifdef DEBUG
  Serial.println("go left slow!");
#endif
}

void Car::turnRight(int duration)
{
  leftForward();
  rightBackward();

  if (duration) {
    startTimer(duration);
  }
#ifdef DEBUG
  Serial.println("go right fast!");
#endif
}

void Car::moveRight(int duration)
{
  leftForward();

  if (duration) {
    startTimer(duration);
  }
#ifdef DEBUG
  Serial.println("go right slow!");
#endif
}

void Car::stop()
{
  stopRight();
  stopLeft();

#ifdef DEBUG
  Serial.println("STOP!");
#endif
}

void Car::leftForward(uint8_t speed) {
  #ifdef DEBUG
  Serial.println("left Forward");
  Serial.println(speed);
  #endif
int actualSpeed = (speed * maxSpeed)/100;
  analogWrite(leftSpeed, actualSpeed);
  digitalWrite(left1, LOW);
  digitalWrite(left2, HIGH);
}

void Car::stopLeft() {
  digitalWrite(leftSpeed, LOW);
}

void Car::leftBackward(uint8_t speed) {
  #ifdef DEBUG
  Serial.println("left Backward");
  Serial.println(speed);
  #endif
  int actualSpeed = (speed * maxSpeed)/100;
  analogWrite(leftSpeed, actualSpeed);
  digitalWrite(left1, HIGH);
  digitalWrite(left2, LOW);
}

void Car::rightForward(uint8_t speed) {
 #ifdef DEBUG
  Serial.print("right forward");
  Serial.println(speed);
  #endif
  int actualSpeed = (speed * maxSpeed)/100;
  analogWrite(rightSpeed, actualSpeed);
  digitalWrite(right1, HIGH);
  digitalWrite(right2, LOW);
}

void Car::stopRight() {
  digitalWrite(rightSpeed, LOW);
}

void Car::rightBackward(uint8_t speed) {
  #ifdef DEBUG
  Serial.print("right backward");
  Serial.println(speed);
  #endif
  int actualSpeed = (speed * maxSpeed)/100;
  
  analogWrite(rightSpeed, actualSpeed);
  digitalWrite(right1, LOW);
  digitalWrite(right2, HIGH);
}
