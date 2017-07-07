#include "Adafruit_SSD1306_RK.h"
#include "Moisture.h"
#include "Temperature.h"
#include "Light.h"

//Display settings
#define OLED_DC     D3
#define OLED_CS     D4
#define OLED_RESET  D5
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);

//Initializing objects
Moisture moisture(A0, D2);
Temperature temperature;
Light light(A1);

long timeout = 1800000; //Time interval (miliseconds) between moisture check
volatile unsigned long lastMicros;
const int waterPump = D6;

void setup() {
  Serial.begin(9600);
  pinMode(waterPump, OUTPUT);

  display.begin(SSD1306_SWITCHCAPVCC);
}

void loop() {
  if((long)(micros() - lastMicros) >= timeout * 1000) {
    lastMicros = micros();
    moisture.read();
    if (moisture.get() < (moisture.getMax() / 2)) {
      digitalWrite(waterPump, HIGH);
      delay(2000);
      digitalWrite(waterPump, LOW);
    }
  }
  display.clearDisplay();
  drawGrid();
  moisture.display(&display);
  temperature.display(&display);
  light.read();
  light.display(&display);
  display.display();
  delay(1000);
}

void drawGrid() {
  display.drawLine(display.width() / 2, 0, display.width() / 2, display.height(), WHITE);
  display.drawLine(display.width()/2, display.height()/2, display.width(), display.height()/2, WHITE);
}
