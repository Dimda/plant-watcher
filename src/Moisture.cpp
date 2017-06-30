#include "Moisture.h"

Moisture::Moisture(int sensorPin, int sensorCurrentPin)
{
  pinMode(sensorPin, INPUT);
  pinMode(sensorCurrentPin, OUTPUT);
  _sensorPin = sensorPin;
  _sensorCurrentPin = sensorCurrentPin;
  _max = 1800;
  _level = 0;

  Particle.variable("moistureLvl", _level);
  Particle.function("setMode", [this](const String& mode)->int{return setMode(mode);});
}

void Moisture::_drawBar(Adafruit_SSD1306* display) {
  int leftPadding = 3;
  float ratio;

  if (_level > _max) {
    ratio = 1.0;
  } else {
    ratio = _level / _max;
  }

  int defaultTopPadding = 10;
  int defaultBarLength = display->height() - defaultTopPadding;
  int topPadding = defaultTopPadding + ((1 - ratio) * defaultBarLength);
  int barLength = display->height() - topPadding;
  int barWidth = 48;

  display->drawRect(leftPadding, defaultTopPadding, barWidth, display->height() - defaultTopPadding, WHITE);

  int barMiddle = ((display->height() - defaultTopPadding) / 2) + defaultTopPadding;
  display->drawLine(leftPadding, barMiddle, leftPadding + barWidth, barMiddle, WHITE);

  display->fillRect(leftPadding, topPadding, barWidth, barLength, WHITE);
}

int Moisture::setMode(String mode) {
  Serial.println("Switched mode to ");
  Serial.println(mode);
  switch (mode.toInt()) {
    case 0:
      _max = 3000.0f;
      break;
    case 1:
      _max = 2000.0f;
      break;
    case 2:
      _max = 1000.0f;
      break;
  }
  return 0;
}

int Moisture::get() {
  return _level;
}

int Moisture::getMax() {
  return _max;
}

void Moisture::read() {
  digitalWrite(_sensorCurrentPin, HIGH);
  delay(100);
  _level = analogRead(_sensorPin);
  digitalWrite(_sensorCurrentPin, LOW);
}

void Moisture::display(Adafruit_SSD1306* display) {
  display->drawChar(5, 0, 'M', WHITE, BLACK, 1);
  display->drawChar(15, 0, 'O', WHITE, BLACK, 1);
  display->drawChar(25, 0, 'I', WHITE, BLACK, 1);
  display->drawChar(35, 0, 'S', WHITE, BLACK, 1);
  display->drawChar(45, 0, 'T', WHITE, BLACK, 1);

  _drawBar(display);
}
