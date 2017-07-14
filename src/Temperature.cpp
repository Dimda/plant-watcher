#include "Temperature.h"

Temperature::Temperature(int sensorPin)
{
  pinMode(sensorPin, INPUT);
  _sensorPin = sensorPin;
  _temperature = 0;
}

void Temperature::read() {
  _temperature = ((analogRead(_sensorPin) * 3.3)/4095) * 100;
}

void Temperature::display(Adafruit_SSD1306* display) {
  display->drawChar(75, 0, 'T', WHITE, BLACK, 1);
  display->drawChar(85, 0, 'E', WHITE, BLACK, 1);
  display->drawChar(95, 0, 'M', WHITE, BLACK, 1);
  display->drawChar(105, 0, 'P', WHITE, BLACK, 1);

  char temperatureChars[3];
  String temp = String(_temperature);
  temp.toCharArray(temperatureChars, 3);

  display->drawChar(75, 10, temperatureChars[0], WHITE, BLACK, 2);
  display->drawChar(88, 10, temperatureChars[1], WHITE, BLACK, 2);
  display->drawCircle(102, 12, 2, WHITE);
  display->drawChar(107, 10, 'C', WHITE, BLACK, 2);
}
