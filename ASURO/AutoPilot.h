#ifndef AUTOPILOT_H
#define AUTOPILOT_H

#include "Car.h"
#include "Timer.h"
#include <Arduino.h>

class AutoPilot{
public:
AutoPilot(int switchPin, int pullupPin);
void controlCar(Car& car);
void startNextAction(Car& car, bool crashDetected);
bool collisionDetected();

private:
 void actionDelay(int duration);
 void setPins();
 Timer actionTimer;
 bool backingUp;
 void turnToRandomDirection(Car& car);
 
 int Switches;
 int SwitchPullup;
 
};

#endif
