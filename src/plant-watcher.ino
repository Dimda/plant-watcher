#include "Adafruit_SSD1306.h"
#include "Adafruit_BMP085.h"

//Display settings
#define OLED_DC     D3
#define OLED_CS     D4
#define OLED_RESET  D5
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);
Adafruit_BMP085 bmp;

int moistureLevel = 0;
long timeout = 1800000; //Time interval (miliseconds) between moisture check
volatile unsigned long lastMicros;

const int moistureSensor = A0;
const int moistureSensorCurrent = D2;
const int waterPump = D6;

float maxMoisture = 1800.0f;

int setMode(String mode);

void setup() {
  Serial.begin(9600);
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
  }
  //Initialize pins
  pinMode(moistureSensor, INPUT);
  pinMode(moistureSensorCurrent, OUTPUT);
  pinMode(waterPump, OUTPUT);

  //Initialize cloud variables
  Particle.variable("moistureLvl", moistureLevel);

  //Initialize display
  display.begin(SSD1306_SWITCHCAPVCC);

  //Initialize cloud functions
  Particle.function("setMode", setMode);
}

void loop() {
  if((long)(micros() - lastMicros) >= timeout * 1000) {
    lastMicros = micros();
    readMoisture();
    if (moistureLevel < (maxMoisture / 2)) {
      digitalWrite(waterPump, HIGH);
      delay(2000);
      digitalWrite(waterPump, LOW);
    }
  }
  display.clearDisplay();
  drawGrid();
  displayMoisture(moistureLevel);
  displayTemperature(String(bmp.readTemperature()));
  display.display();
  delay(1000);
}

int setMode(String mode) {
  switch (mode.toInt()) {
    case 0:
      maxMoisture = 3000.0f;
      break;
    case 1:
      maxMoisture = 2000.0f;
      break;
    case 2:
      maxMoisture = 1000.0f;
      break;
  }
}

void drawGrid() {
  display.drawLine(display.width() / 2, 0, display.width() / 2, display.height(), WHITE);
  display.drawLine(display.width()/2, display.height()/2, display.width(), display.height()/2, WHITE);
}

void drawBar(int moistureLevel) {
  int leftPadding = 3;
  float moistureRatio;

  if (moistureLevel > maxMoisture) {
    moistureRatio = 1.0;
  } else {
    moistureRatio = moistureLevel / maxMoisture;
  }

  int defaultTopPadding = 10;
  int defaultBarLength = display.height() - defaultTopPadding;
  int topPadding = defaultTopPadding + ((1 - moistureRatio) * defaultBarLength);
  int barLength = display.height() - topPadding;
  int barWidth = 48;

  display.drawRect(leftPadding, defaultTopPadding, barWidth, display.height() - defaultTopPadding, WHITE);

  int barMiddle = ((display.height() - defaultTopPadding) / 2) + defaultTopPadding;
  display.drawLine(leftPadding, barMiddle, leftPadding + barWidth, barMiddle, WHITE);

  display.fillRect(leftPadding, topPadding, barWidth, barLength, WHITE);
}

void displayMoisture(int moistureLevel) {
  display.drawChar(5, 0, 'M', WHITE, BLACK, 1);
  display.drawChar(15, 0, 'O', WHITE, BLACK, 1);
  display.drawChar(25, 0, 'I', WHITE, BLACK, 1);
  display.drawChar(35, 0, 'S', WHITE, BLACK, 1);
  display.drawChar(45, 0, 'T', WHITE, BLACK, 1);

  drawBar(moistureLevel);
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

void readMoisture() {
  digitalWrite(moistureSensorCurrent, HIGH);
  delay(100);
  moistureLevel = analogRead(moistureSensor);
  digitalWrite(moistureSensorCurrent, LOW);
}
