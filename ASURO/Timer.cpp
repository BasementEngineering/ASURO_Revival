#include "Timer.h"

Timer::Timer(int milliseconds, bool AutoReset) {
  active = false;
  interval = milliseconds;
  autoReset = AutoReset;
  startTime = millis();
}

void Timer::start(){ 
  active = true;
  reset();
}

void Timer::stop(){
  active = false;
}

void Timer::reset() {
  startTime = millis();
}

void Timer::setInterval(int milliSeconds) {
  interval = milliSeconds;
}

void Timer::setAutoReset(bool mode){
  autoReset = mode;
}

bool Timer::isActive(){
  return active;
}

bool Timer::isFinished() {
  if(active){
  if (millis() - startTime > interval) {
    if (autoReset) {
      reset();
    }
    return true;
  }
  }
  return false;
}

