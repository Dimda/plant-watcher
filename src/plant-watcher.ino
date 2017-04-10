#include "Adafruit_SSD1306.h"

//Display settings
#define OLED_DC     D3
#define OLED_CS     D4
#define OLED_RESET  D5
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);

int moistureLevel = 0;
void setup()   {
  //Initialize moisture sensor
  pinMode(A0, INPUT);
  Particle.variable("moistureLvl", moistureLevel);

  //Initialize display
  display.begin(SSD1306_SWITCHCAPVCC);
}

void loop() {
  moistureLevel = analogRead(A0);
  displayMoisture(moistureLevel);
  delay(2000);
}

void displayMoisture(int moistureLevel) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Moisture:");
  display.println(moistureLevel);
  display.display();
}
