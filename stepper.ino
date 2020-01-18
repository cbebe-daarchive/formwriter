#include <Arduino.h>

void setup() {  
  for (int i = 4; i < 8; ++i {
    pinMode(i, INPUT);
    digitalWrite(i, LOW);
  }
}

void loop() {
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 4; j++) {
      digitalWrite(j + 4, i & (1 << j));
    }
  }
}

