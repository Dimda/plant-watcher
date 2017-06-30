#include "Adafruit_SSD1306_RK.h"
#include "Adafruit_BMP085.h"
#include "Moisture.h"

//Display settings
#define OLED_DC     D3
#define OLED_CS     D4
#define OLED_RESET  D5
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);
Adafruit_BMP085 bmp;
Moisture moisture(A0, D2);

long timeout = 1800000; //Time interval (miliseconds) between moisture check
volatile unsigned long lastMicros;

const int waterPump = D6;
const int photoresistor = A1;

/*int setMode(String mode);*/

void setup() {
  Serial.begin(9600);
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
  }
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
  displayTemperature(String(bmp.readTemperature()));
  displayLight(analogRead(photoresistor));
  display.display();
  delay(1000);
}

void drawGrid() {
  display.drawLine(display.width() / 2, 0, display.width() / 2, display.height(), WHITE);
  display.drawLine(display.width()/2, display.height()/2, display.width(), display.height()/2, WHITE);
}

void displayTemperature(String temperature) {
  display.drawChar(75, 0, 'T', WHITE, BLACK, 1);
  display.drawChar(85, 0, 'E', WHITE, BLACK, 1);
  display.drawChar(95, 0, 'M', WHITE, BLACK, 1);
  display.drawChar(105, 0, 'P', WHITE, BLACK, 1);

  char temperatureChars[3];
  temperature.toCharArray(temperatureChars, 3);

  display.drawChar(75, 10, temperatureChars[0], WHITE, BLACK, 2);
  display.drawChar(88, 10, temperatureChars[1], WHITE, BLACK, 2);
  display.drawCircle(102, 12, 2, WHITE);
  display.drawChar(107, 10, 'C', WHITE, BLACK, 2);
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
