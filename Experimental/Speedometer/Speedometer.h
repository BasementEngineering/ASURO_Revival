#ifndef SPEEDOMETER_H
#define SPEEDOMETER_H

#include "Timer.h"

class Speedometer
{
public:
Speedometer(int sensorPin);

int getSpeed(void) const; //mm/s

void update(void);
void calibrate(void);
bool isCalibrating(void);

private:
int sensor;
const int stepDistance = 2; //mm
int speed;
int minReading;
int maxReading;
int previousMark;

Timer updateTimer;
Timer stopWatch;
Timer calibrationTimer;

enum states{WHITE_MARK, BLACK_MARK, NO_MARK_DETECTED};
int calculateMark(int reading);
void calculateSpeed(int mark);
void measureMinMaxValues(int reading);

};  

#endif
