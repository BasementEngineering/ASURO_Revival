#ifndef MOUTH_H
#define MOUTH_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Timer.h>

class Mouth
{
  public:
  Mouth(void);
  void begin();

  void update();

  void startTalking(void);
  void stopTalking(void);
  void smile(void);
  void grin(void);

  private:
  bool isTalking;
  bool mouthOpen;
  
  Timer updateTimer;
  
  const int OLED_RESET = 4;
  Adafruit_SSD1306 display;

  void drawClosedMouth(void);
};


#endif
