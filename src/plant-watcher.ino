#include "Adafruit_SSD1306_RK.h"
#include "Moisture.h"
#include "Temperature.h"

//Display settings
#define OLED_DC     D3
#define OLED_CS     D4
#define OLED_RESET  D5
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);
Moisture moisture(A0, D2);
Temperature temperature;

long timeout = 1800000; //Time interval (miliseconds) between moisture check
volatile unsigned long lastMicros;

const int waterPump = D6;
const int photoresistor = A1;

void setup() {
  Serial.begin(9600);
  pinMode(waterPump, OUTPUT);
  pinMode(photoresistor, INPUT);

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
  displayLight(analogRead(photoresistor));
  display.display();
  delay(1000);
}

void drawGrid() {
  display.drawLine(display.width() / 2, 0, display.width() / 2, display.height(), WHITE);
  display.drawLine(display.width()/2, display.height()/2, display.width(), display.height()/2, WHITE);
}

void drawSunIcon(int x, int y, int radius) {
  display.drawCircle(x, y, radius, WHITE);

  for (int i = 0; i < 360; i +=  30 ) {
    float rad = i * 1000.0 / 57296.0;
    float lineStartX = x + (radius + 2) * sin(rad);
    float lineStartY = y + (radius + 2) * cos(rad);
    float lineEndX = x + (radius + 5) * sin(rad);
    float lineEndY = y + (radius + 5) * cos(rad);
    display.drawLine(lineStartX, lineStartY, lineEndX, lineEndY, WHITE);
  }
}

void displayLight(int lightLevel) {
  display.drawChar(75, 34, 'L', WHITE, BLACK, 1);
  display.drawChar(85, 34, 'I', WHITE, BLACK, 1);
  display.drawChar(95, 34, 'G', WHITE, BLACK, 1);
  display.drawChar(105, 34, 'H', WHITE, BLACK, 1);
  display.drawChar(115, 34, 'T', WHITE, BLACK, 1);

  if (lightLevel < 5) {
    drawSunIcon(77, 52, 5);
    drawSunIcon(97, 52, 5);
    drawSunIcon(117, 52, 5);
  } else if (lightLevel < 15) {
    drawSunIcon(77, 52, 5);
    drawSunIcon(97, 52, 5);
  } else if (lightLevel < 25) {
    drawSunIcon(77, 52, 5);
  }
}
