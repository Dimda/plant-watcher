#ifndef Light_h
#define Light_h
#include "Adafruit_SSD1306_RK.h"

class Light
{
  public:
    Light(int photoresistorPin);
    void display(Adafruit_SSD1306* display);
    void read();
  private:
    int _photoresistorPin;
    int _level;
    void _drawSunIcon(Adafruit_SSD1306* display, int x, int y, int radius);
};
#endif
