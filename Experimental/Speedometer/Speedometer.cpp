#include "Speedometer.h"
#include "Arduino.h"

Speedometer::Speedometer(int sensorPin):
updateTimer(10,true), stopWatch(500,true), calibrationTimer(5000,false)
{
  updateTimer.start();

  sensor = sensorPin;
  pinMode(sensorPin,INPUT);

  speed = 0;
  minReading = 1024;
  maxReading = 0;
  
  previousMark = NO_MARK_DETECTED;
}

int Speedometer::getSpeed(void) const
{
  return speed;
}

void Speedometer::calibrate(void)
{
  Serial.println("Starting Calibration");
  calibrationTimer.start();
  stopWatch.stop();
}

bool Speedometer::isCalibrating(void)
{
  return calibrationTimer.isActive();
}

void Speedometer::update(void)
{
  if(updateTimer.isFinished())
  {
	  
  int reading = analogRead(sensor);

  if(!calibrationTimer.isActive())
  {
	int mark = calculateMark(reading);
    calculateSpeed(mark);
  }
  else
  {
	if(!calibrationTimer.isFinished())
	{
		measureMinMaxValues(reading);
    }
	else
    {
	calibrationTimer.stop();
    stopWatch.start();	
    }
  }
  }
}

int Speedometer::calculateMark(int reading)
{
	int range = maxReading - minReading;
	int correctedReading = reading - minReading;
	
	if(correctedReading > range * 0.8)
	{
		return WHITE_MARK;
    }
	else if(correctedReading < range * 0.2)
	{
		return BLACK_MARK;
    }
	else
	{
	return NO_MARK_DETECTED;	
	}	
}

void Speedometer::calculateSpeed(int mark)
{
	if(mark == NO_MARK_DETECTED)
	{
	  return;	
	}	
	
	if(mark != previousMark)
	{
    Serial.println("Changed");
	  previousMark = mark;
	  if(!stopWatch.isFinished())
      {
	  unsigned long time = stopWatch.getRunTime();
	  if(time != 0)
	  {
	  speed = stepDistance / time;	
	  }
	  }
	  else
	  {
	  speed = 0;
	  }
	  stopWatch.reset();
	}	
}	

void Speedometer::measureMinMaxValues(int reading)
{
  Serial.print("Reading: ");
  Serial.println(reading);
  if(reading < minReading)
    {
      minReading = reading;
    }
    if(reading > maxReading)
    {
      maxReading = reading;
    }
    
}
