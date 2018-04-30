#include "Mouth.h"
#include "Images.h"

Mouth::Mouth(void):
display(OLED_RESET), updateTimer(400,true) 
{
  updateTimer.start();
  isTalking = false;
  mouthOpen = false;
}

void Mouth::begin(void)
{
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
}

void Mouth::update()
{
  if(updateTimer.isFinished() && isTalking)
  {
    display.clearDisplay();
    if(mouthOpen)
    {
      drawClosedMouth();
      mouthOpen = false;    
    }
    else
    {
      display.drawBitmap(8, 2,  OpenMouth, 112, 32, 1);
        mouthOpen = true; 
    }
    display.display();
  }
}

void Mouth::startTalking(void)
{
  mouthOpen = true;
isTalking = true;  
}

void Mouth::stopTalking()
{
  drawClosedMouth();
  mouthOpen = false;
  isTalking = false;
}

void Mouth::smile(void)
{
  display.clearDisplay();
  display.drawBitmap(8, 0, Smile, 112, 24, 1);
  display.display();
  mouthOpen = false;
  isTalking = false;
}

void Mouth::grin(void)
{
  display.clearDisplay();
  display.drawBitmap(8, 2,  Grin, 112, 24, 1);
  display.display();
  mouthOpen = false;
  isTalking = false;
}

void Mouth::drawClosedMouth(void)
{
  int y0 = 14;
  int x0 = 8;
  display.clearDisplay();
  for(int i = 0; i < 6; i++)
  {
    display.drawFastHLine(x0,(y0 + i), 112, 1);
  }
  display.display();
}

