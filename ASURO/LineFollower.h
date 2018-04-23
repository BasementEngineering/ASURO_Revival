#ifndef LINEFOLLOWER_H
#define LINEFOLLOWER_H

#include "Car.h"
#include <Arduino.h>

class LineFollower{
  public:
 LineFollower(int ledPin, int leftSensorPin, int rightSensorPin, int _tolerance);
 void controlCar(Car& car);
 
 void calibrateSensors();
 void reset();

 private:
 void setPins();
 void updateValues();
 void blinkLED(int times, int interval);
 bool onLine(char side);
 bool withinTolerance(int currentValue, int setPoint);

 int LED;
 int LeftSensor;
 int RightSensor;
 
 int leftValue;
 int rightValue;
 
 int tolerance;
 char previousLineLocation;

 int rightBlackValue;
 int rightWhiteValue;
 int leftBlackValue;
 int leftWhiteValue;
 
 Timer timeoutTimer;
};

#endif
