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
Moisture moisture(A2, D2);
Temperature temperature(A0);
Light light(A1);

const int wateringTimeout = 1800000; //Time interval (miliseconds) between moisture check
volatile unsigned long lastTimeWatered;
const int debouncingTime = 300;// Debouncing time (miliseconds) for button clicks
volatile unsigned long lastTimeClicked;
const int waterPump = D6;
const int button = D1;
bool useDisplay = true;

void setup() {
  Serial.begin(9600);
  pinMode(waterPump, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  attachInterrupt(button, toggleDisplay, CHANGE);

  display.begin(SSD1306_SWITCHCAPVCC);
}

void loop() {
  if((long)(micros() - lastTimeWatered) >= wateringTimeout * 1000) {
    lastTimeWatered = micros();
    moisture.read();
    if (moisture.get() < (moisture.getMax() / 2)) {
      digitalWrite(waterPump, HIGH);
      delay(2000);
      digitalWrite(waterPump, LOW);
    }
  }
  display.clearDisplay();
  if (useDisplay == true) {
    drawGrid();
    moisture.display(&display);
    temperature.read();
    temperature.display(&display);
    light.read();
    light.display(&display);
  }
  display.display();
  delay(1000);
}

void drawGrid() {
  display.drawLine(display.width() / 2, 0, display.width() / 2, display.height(), WHITE);
  display.drawLine(display.width()/2, display.height()/2, display.width(), display.height()/2, WHITE);
}

void toggleDisplay() {
  if ((long)(micros() - lastTimeClicked) >= debouncingTime * 1000) {
    useDisplay = !useDisplay;
    lastTimeClicked = micros();
  }
}
