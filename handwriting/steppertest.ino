#include <Stepper.h>

#define LEFT_CW 8
#define LEFT_CCW 9 
#define RIGHT_CW 2
#define RIGHT_CCW 3

Stepper leftStep = Stepper(32, 13, 12, 11, 10);
Stepper rightStep = Stepper(32, 55, 54, 53, 52);


void setup() {
  int pushButtons[] = {8, 9, 39, 40};
  for (int i = 0; i < 2; i++) {
    pinMode(pushButtons[i], INPUT);
    digitalWrite(pushButtons[i], HIGH);
  }

  leftStep.setSpeed(8); rightStep.setSpeed(8);
  Serial.begin(9600); Serial3.begin(9600);
}
void loop() {
  if (digitalRead(LEFT_CW) == LOW) {
    leftStep.step(-1);
  } else if (digitalRead(LEFT_CCW) == LOW) {
    leftStep.step(1);
  }
  if (digitalRead(RIGHT_CW) == LOW) {
    rightStep.step(-1);
  } else if (digitalRead(RIGHT_CCW) == LOW) {
    rightStep.step(1);
  }
}
