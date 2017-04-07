int moistureLevel = 0;
void setup() {
  pinMode(A4, INPUT);
  Particle.variable("moistureLvl", moistureLevel);
}

void loop() {
  moistureLevel = analogRead(A4);
  delay(200);
}
