
#include "Mouth.h"
#include <Timer.h>

#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>
#include <Servo.h>

Mouth mouth;
Servo myServo;  
Timer nextAnimationTimer(5000,true);
bool isTalking = false;

const int trigPin = 6;
const int echoPin = 7;
const int servoPin = 4;
const int ledPin = 5;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(2, ledPin, NEO_GRB + NEO_KHZ800);

void setup() {
Serial.begin(9600); // Starts the serial communication
mouth.begin();
mouth.smile();
delay(2000);
mouth.grin();
delay(2000);
nextAnimationTimer.start();  
startServo();
startStrip();
Serial.println("finished setup");  
}

void loop() 
{
  if(nextAnimationTimer.isFinished())
  {
    if(isTalking)
    {
      Serial.println("Stopping Talking");  
      mouth.stopTalking();
      isTalking = false;
    }
    else
    {
      mouth.startTalking();
      Serial.println("Starting Talking");  
      isTalking = true;
    }
  }
  mouth.update();
}

void startServo(void)
{
    myServo.attach(servoPin); // attaches the servo on pin 9 to the servo object
   myServo.write(90);
    myServo.detach();
}

void startDisplay(void)
{
    // initialize with the I2C addr 0x3C (for the 128x32)


 
}

void setUpDistanceSensor(void)
{
   pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
   pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}

void startStrip(void)
{
 strip.begin();
  strip.show();
  strip.setPixelColor(0, strip.Color(255,255,255));
  strip.setPixelColor(1, strip.Color(255,255,255));
 
  strip.show(); 
   delay(1000);
   strip.clear();
  strip.show();
}

int getDistance(void)
{
  // Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
long duration = pulseIn(echoPin, HIGH);
// Calculating the distance
int distance= duration*0.034/2;
// Prints the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.println(distance);
return distance;
}

