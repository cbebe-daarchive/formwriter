#include <Stepper.h>

#define LEFT_CW 12
#define LEFT_CCW 13 
#define RIGHT_CW 2
#define RIGHT_CCW 3
const int stepsPerRevolution = 4076;

Stepper leftStep = Stepper(stepsPerRevolution, 8, 9, 10, 11);
Stepper rightStep = Stepper(stepsPerRevolution, 4, 5, 6, 7);


void setup() {
  int pushButtons[] = {2, 3, 12, 13};
  for (int i = 0; i < 4; i++) {
    pinMode(pushButtons[i], INPUT);
    digitalWrite(pushButtons[i], HIGH);
  }
  
  leftStep.setSpeed(170);
  
  // Begin Serial communication at a baud rate of 9600:
  Serial.begin(9600);
  Serial3.begin(9600);
}
void loop() {
  if (digitalRead(LEFT_CW) == 1) {
    
  }
  leftStep.step(1);
}
