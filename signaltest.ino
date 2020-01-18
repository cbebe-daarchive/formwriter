#include <Arduino.h>

void setup() {
  pinMode(13,INPUT);
  digitalWrite(13, LOW);
  Serial.begin(9600);
}

void loop() {
  if (digitalRead(13) == HIGH) {
    Serial.println("oop");
  } 
}