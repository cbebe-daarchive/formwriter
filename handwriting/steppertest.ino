#include <Stepper.h>

#define LEFT_CW 12
#define LEFT_CCW 13 
#define RIGHT_CW 2
#define RIGHT_CCW 3
const int stepsPerRevolution = 4076;

Stepper leftStep = Stepper(stepsPerRevolution, 11, 10, 9, 8);
Stepper rightStep = Stepper(stepsPerRevolution, 4, 5, 6, 7);


void setup() {
  int pushButtons[] = {2, 3, 12, 13};
  for (int i = 0; i < 4; i++) {
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