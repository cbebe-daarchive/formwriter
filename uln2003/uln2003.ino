#include <AccelStepper.h>
#define HALFSTEP 8

#define STEPPER_MAX_SPEED 3000
#define STEPPER_FAST STEPPER_MAX_SPEED
#define STEPPER_SLOW (STEPPER_MAX_SPEED/2)

// Motor pin definitions
#define PIN_MOTOR1_IN1  4     // IN1 on the ULN2003 driver 1
#define PIN_MOTOR1_IN2  5     // IN2 on the ULN2003 driver 1
#define PIN_MOTOR1_IN3  6    // IN3 on the ULN2003 driver 1
#define PIN_MOTOR1_IN4  7    // IN4 on the ULN2003 driver 1

AccelStepper stepperX(HALFSTEP, PIN_MOTOR1_IN1, PIN_MOTOR1_IN3, PIN_MOTOR1_IN2, PIN_MOTOR1_IN4);

void setup() {

  stepperX.setMaxSpeed(STEPPER_MAX_SPEED);
  stepperX.setAcceleration(300.0);
  stepperX.setSpeed(STEPPER_FAST);
      stepperX.enableOutputs();
//  stepperX.disableOutputs();
}

void loop() {
  // put your main code here, to run repeatedly:
  int i = 1;
  while (1) {
    
    stepperX.moveTo(i*5000);
        if (abs(stepperX.distanceToGo()) > 1000)
          while (abs(stepperX.distanceToGo()) > 1000) stepperX.run();
        else if (abs(stepperX.distanceToGo()) > 100)
          while (abs(stepperX.distanceToGo()) > 100) stepperX.run();
        else if (abs(stepperX.distanceToGo()) > 50)
          while (abs(stepperX.distanceToGo()) > 50) stepperX.run();
        else
          stepperX.run();
        i -= 2*i;
  }
}
