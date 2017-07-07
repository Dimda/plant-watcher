#include "Light.h"

Light::Light(int photoresistorPin)
{
  pinMode(photoresistorPin, INPUT);
  _photoresistorPin = photoresistorPin;
  _level = 0;
}

void Light::_drawSunIcon(Adafruit_SSD1306* display, int x, int y, int radius) {
  display->drawCircle(x, y, radius, WHITE);

  for (int i = 0; i < 360; i +=  30 ) {
    float rad = i * 1000.0 / 57296.0;
    float lineStartX = x + (radius + 2) * sin(rad);
    float lineStartY = y + (radius + 2) * cos(rad);
    float lineEndX = x + (radius + 5) * sin(rad);
    float lineEndY = y + (radius + 5) * cos(rad);
    display->drawLine(lineStartX, lineStartY, lineEndX, lineEndY, WHITE);
  }
}

void Light::read() {
  _level = analogRead(_photoresistorPin);
}

void Light::display(Adafruit_SSD1306* display) {
  display->drawChar(75, 34, 'L', WHITE, BLACK, 1);
  display->drawChar(85, 34, 'I', WHITE, BLACK, 1);
  display->drawChar(95, 34, 'G', WHITE, BLACK, 1);
  display->drawChar(105, 34, 'H', WHITE, BLACK, 1);
  display->drawChar(115, 34, 'T', WHITE, BLACK, 1);

  if (_level < 5) {
    _drawSunIcon(display, 77, 52, 5);
    _drawSunIcon(display, 97, 52, 5);
    _drawSunIcon(display, 117, 52, 5);
  } else if (_level < 15) {
    _drawSunIcon(display, 77, 52, 5);
    _drawSunIcon(display, 97, 52, 5);
  } else if (_level < 25) {
    _drawSunIcon(display, 77, 52, 5);
  }
}
