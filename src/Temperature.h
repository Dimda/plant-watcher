#ifndef Temperature_h
#define Temperature_h
#include "Adafruit_SSD1306_RK.h"

class Temperature
{
  public:
    Temperature(int sensorPin);
    void display(Adafruit_SSD1306* display);
    void read();
  private:
    int _sensorPin;
    int _temperature;
};
#endif
