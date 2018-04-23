#include "Speedometer.h"
#include "Arduino.h"

Speedometer::Speedometer(int Led, int leftSensor, int rightSensor):
updateTimer(20,true)
{
  ledPin = Led;
  leftPin = leftSensor;
  rightPin = rightSensor;
  pinMode(Led,OUTPUT);
  pinMode(leftPin,INPUT);
  pinMode(rightPin,INPUT);

  calibrating = false;

  leftSpeed = 0;
  leftMinReading = 1024;
  leftMaxReading = 0;

  rightSpeed = 0;
  rightMinReading = 1024;
  rightMaxReading = 0;
}

int Speedometer::getLeftSpeed(void)
{
  return rightSpeed;
}

int Speedometer::getRightSpeed(void)
{
  return leftSpeed;
}

int Speedometer::getSpeed(void){
  return (leftSpeed + rightSpeed) / 2;
}

void enterCalibrationMode(void)
{
  calibrating = true;
}

void endCalibrationMode(void)
{
  calibrating = false;
}

void Speedometer::update(void)
{
  if(updateTimer.isFinished())
  {
  int leftReading = analogRead(leftPin);
  int rightReading = analogRead(rightPin);

  if(!calibrating)
  {
    
  }
  else
  {
    setMinMaxValues(leftReading,rightReading);
  }
  }
}

void setMinMaxValues(int leftReading, int rightReading)
{
  if(leftReading < leftMinReading)
    {
      leftMinReading = leftReading;
    }
    if(leftReading > leftMaxReading)
    {
      leftMaxReading = leftReading;
    }
    
    if(rightReading < rightMinReading)
    {
      rightMinReading = rightReading;
    }
    if(rightReading > rightMaxReading)
    {
      rightMaxReading = rightReading;
    }
}

int leftSpeed;
int leftMinReading;
int leftMaxReading;

int rightSpeed;
int rightMinReading;
int rightMaxReading;
