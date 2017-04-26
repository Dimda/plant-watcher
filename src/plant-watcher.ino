#include "Adafruit_SSD1306.h"

//Display settings
#define OLED_DC     D3
#define OLED_CS     D4
#define OLED_RESET  D5
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);

int moistureLevel1 = 0;
int moistureLevel2 = 0;

int moistureSensor1 = A0;
int moistureSensor2 = A1;
int waterPump = D0;

void setup()   {
  //Initialize pins
  pinMode(moistureSensor1, INPUT);
  pinMode(moistureSensor2, INPUT);
  pinMode(waterPump, OUTPUT);

  //Initialize cloud variables
  Particle.variable("moistureLvl1", moistureLevel1);
  Particle.variable("moistureLvl2", moistureLevel2);

  //Initialize display
  display.begin(SSD1306_SWITCHCAPVCC);
}

void loop() {
  moistureLevel1 = analogRead(moistureSensor1);
  moistureLevel2 = analogRead(moistureSensor2);
  displayMoisture(moistureLevel1, moistureLevel2);
  controlWaterPump(moistureLevel1, moistureLevel2);
  delay(500);
}

void drawBar(String position, int moistureLevel) {
  int leftPadding;
  if (position == "left") {
    leftPadding = 8;
  } else {
    leftPadding = 72;
  }

  float moistureRatio;
  if (moistureLevel > 1000) {
    moistureRatio = 1.0;
  } else {
    moistureRatio = moistureLevel / 1000.0f;
  }

  int defaultTopPadding = 10;
  int defaultBarLength = display.height() - defaultTopPadding;
  int topPadding = defaultTopPadding + ((1 - moistureRatio) * defaultBarLength);
  int barLength = display.height() - topPadding;
  int barWidth = 48;

  display.drawRect(leftPadding, defaultTopPadding, barWidth, display.height() - defaultTopPadding, WHITE);
  display.fillRect(leftPadding, topPadding, barWidth, barLength, WHITE);

}

void displayMoisture(int moistureLevel1, int moistureLevel2) {
  display.clearDisplay();
  display.drawLine(display.width() / 2, 0, display.width() / 2, display.height(), WHITE);
  display.drawChar(25, 0, 'M', WHITE, BLACK, 1);
  display.drawChar(34, 0, '1', WHITE, BLACK, 1);
  display.drawChar(89, 0, 'M', WHITE, BLACK, 1);
  display.drawChar(98, 0, '2', WHITE, BLACK, 1);

  drawBar("left", moistureLevel1);
  drawBar("right", moistureLevel2);
  display.display();
}

void controlWaterPump(int moistureLevel1, int moistureLevel2) {
  int averageMoistureLevel = (moistureLevel1 + moistureLevel2)/2;
  if (averageMoistureLevel < 200) {
    digitalWrite(waterPump, HIGH);
  } else {
    digitalWrite(waterPump, LOW);
  }
}
