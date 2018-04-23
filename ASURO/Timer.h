#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

class Timer{
public:
Timer(int milliseconds = 0, bool autoReset = false);

void start();
void reset();
bool isFinished();
void stop();
bool isActive();
void setInterval(int milliseconds);
void setAutoReset(bool mode);

private:
int interval;
unsigned long startTime;
bool autoReset;  
bool active;
};

#endif
