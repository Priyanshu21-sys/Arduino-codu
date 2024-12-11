#include <Wire.h>

#define sensor 34

void setup() {
  // Debug console
  Serial.begin(9600);
}

//Get the gas sensor values
void GASLevel() {
  int value = analogRead(sensor);
  value = map(value, 0, 4095, 0, 100);

  Serial.print("Gas Level: ");
  Serial.println(value);
}

void loop() {
  GASLevel();
  delay(200);
}
