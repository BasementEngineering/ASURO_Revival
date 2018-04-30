//Speedometer
const int OdometerLed = 3;
const int LeftOdoSensor = A3;
const int RightOdoSensor = A4;

const int LeftSpeed = 5;
const int LeftForward = 7;
const int LeftBackward = 8;

const int RightSpeed = 6;
const int RightForward = 9;
const int RightBackward = 10;

#include "Speedometer.h"
#include "Car.h"
#include "Timer.h"

//Objects
Car car(LeftSpeed, LeftForward, LeftBackward, RightSpeed, RightForward, RightBackward);
Speedometer rightSpeedo(RightOdoSensor);
Timer printTimer(1000,true);
Timer testTimer(10000,false);

void setup() {
  Serial.begin(9600);
  Serial.println("Debugging mode active");
  pinMode(OdometerLed,OUTPUT);
  digitalWrite(OdometerLed,HIGH);
  delay(5000);
  
  Serial.println("Starting Speedometer Calibration");
  rightSpeedo.calibrate();
  car.rightForward(100);
  printTimer.start();
  testTimer.start();
}

void loop() {
  car.update();
  rightSpeedo.update();

  if(testTimer.isFinished())
  {
    car.stop();
  }

  if( ( !rightSpeedo.isCalibrating() ) && printTimer.isFinished())
  {
    Serial.print("Current Speed: ");
    Serial.println(rightSpeedo.getSpeed());
  }
    
}
