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

void setup()   {
  //Initialize moisture sensor
  pinMode(moistureSensor1, INPUT);
  pinMode(moistureSensor2, INPUT);
  Particle.variable("moistureLvl1", moistureLevel1);
  Particle.variable("moistureLvl2", moistureLevel2);

  //Initialize display
  display.begin(SSD1306_SWITCHCAPVCC);
}

void loop() {
  moistureLevel1 = analogRead(moistureSensor1);
  moistureLevel2 = analogRead(moistureSensor2);
  displayMoisture(moistureLevel1, moistureLevel2);

  delay(2000);
}

void displayMoisture(int moistureLevel1, int moistureLevel2) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("M1:");
  display.println(moistureLevel1);
  display.println("M2:");
  display.println(moistureLevel2);
  display.display();
}
