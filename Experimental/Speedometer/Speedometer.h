#ifndef SPEEDOMETER_H
#define SPEEDOMETER_H

#include "Timer.h"

class Speedometer
{
public:
Speedometer(int Led, int leftSensor, int rightSensor);

int getLeftSpeed(void); //mm/s
int getRightSpeed(void); //mm/s
int getSpeed(void);

void update(void);
void enterCalibrationMode(void);
void endCalibrationMode(void);

private:
int ledPin;
int LeftPin;
int rightPin;

bool calibrating;

const int stepDistance = 2; //mm
Timer updateTimer;
int updateInterval
//LeftSide
int leftSpeed;
int leftMinReading;
int leftMaxReading;

int rightSpeed;
int rightMinReading;
int rightMaxReading;
};  

#endif
