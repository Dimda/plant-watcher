#ifndef Moisture_h
#define Moisture_h
#include "Adafruit_SSD1306_RK.h"

class Moisture
{
  public:
    Moisture(int sensorPin, int sensorCurrentPin);
    void display(Adafruit_SSD1306* display);
    void read();
    int get();
    int getMax();
    int setMode(String mode);
  private:
    void _drawBar(Adafruit_SSD1306* display);
    int _level;
    float _max;
    int _sensorPin;
    int _sensorCurrentPin;
};
#endif
