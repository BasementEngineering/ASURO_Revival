#ifndef CAR_H
#define CAR_H

#include <Arduino.h>
#include "Timer.h"

class Car {
  public:
    Car(int _leftSpeed, int _left1, int _left2, int _rightSpeed, int _right1, int _right2);
    void update();

void setSpeed(int newSpeed);
bool isMoving();

//Movement functions for Joystick control
void setMaxSpeed(uint8_t newMaxSpeed);
void setDirection(int newDirection);
void setDirectionCorrection(int value);
void startMoving();

//Dual Motor Movements
    void moveForward(int duration = 0);
    void moveBackward(int duration = 0);
    void turnLeft(int duration = 0);
    void moveLeft(int duration = 0);
    void turnRight(int duration = 0);
    void moveRight(int duration = 0);
    void stop();

//Single Motor Movements
    void stopLeft();
    void stopRight();
    void leftForward(uint8_t speed = 255);
    void rightForward(uint8_t speed = 255);
    void leftBackward(uint8_t speed = 255);
    void rightBackward(uint8_t speed = 255);

  private:
    bool carIsMoving;
int direction;
int currentSpeed;

int directionCorrection;
int maxSpeed;
int minSpeed;
    
    Timer actionTimer;
    void startTimer(int duration);

int leftSpeed;
int left1;
int left2;

int rightSpeed;
int right1;
int right2;

};
#endif
