int RightEn = 5;
int RightDir = 4;
int LeftEn = 6;
int LeftDir = 7;

int LeftSen = A1;
int LeftMSen = A2;
int MidSen = A3;
int RightMSen = A4;
int RightSen = A5;

const byte interruptLeftEndPin = 2;
const byte interruptLeftPin = 21;
const byte interruptMiddlePin = 18;
const byte interruptRightPin = 20;
const byte interruptRightEndPin = 3;

//PID constants
int last_proportional = 0;
int integral = 0;
float Kp = 0.08;
float Ki = 0.0002;
float Kd = 1.0;

int con_speed = 130;
int adjust_speed = 100;
int turn_speed = 0;

void setup()
{
  //Motor Driver Pin Setup
  pinMode(RightEn,OUTPUT);
  pinMode(RightDir,OUTPUT);
  pinMode(LeftEn,OUTPUT);
  pinMode(LeftDir,OUTPUT);


  //LSS05 Auto-Calibrating Line Sensor Pin Setup
  pinMode(LeftSen,INPUT);
  pinMode(LeftMSen,INPUT);
  pinMode(MidSen,INPUT);
  pinMode(RightMSen,INPUT);
  pinMode(RightSen,INPUT);

  //Interrupt pin
  pinMode(interruptRightPin, INPUT);
  pinMode(interruptMiddlePin, INPUT);
  pinMode(interruptLeftPin, INPUT);
  pinMode(interruptRightEndPin, INPUT);
  pinMode(interruptLeftEndPin, INPUT);

  analogWrite(RightEn, 0);   //PWM Speed Control
  analogWrite(LeftEn, 0);

  attachInterrupt(digitalPinToInterrupt(interruptRightPin),right, CHANGE);
  attachInterrupt(digitalPinToInterrupt(interruptLeftPin), left, CHANGE);
  attachInterrupt(digitalPinToInterrupt(interruptMiddlePin), forward, CHANGE);
  attachInterrupt(digitalPinToInterrupt(interruptRightEndPin), rightturn, CHANGE);
  attachInterrupt(digitalPinToInterrupt(interruptLeftEndPin), leftturn, CHANGE);
  noInterrupts();
  
  }

void loop()
{
  digitalWrite(RightDir, HIGH);
  analogWrite(RightEn, 0);   //PWM Speed Control
  
  // if((digitalRead(LeftSen)==1)&&(digitalRead(LeftMSen)==1)&&(digitalRead(MidSen)==1)&&(digitalRead(RightMSen)==1)&&(digitalRead(RightSen)==1))
  // {
  // digitalWrite(RightDir,HIGH);
  // digitalWrite(LeftDir,LOW);
  // analogWrite(RightEn, 0);   //PWM Speed Control
  // analogWrite(LeftEn, 0);   //PWM Speed Control 
  // noInterrupts(); //Disable interrupts
  // }
  // else if((digitalRead(LeftSen)==0)&&(digitalRead(LeftMSen)==0)&&(digitalRead(MidSen)==1)&&(digitalRead(RightMSen)==0)&&(digitalRead(RightSen)==0))
  // {
  // digitalWrite(RightDir,HIGH);
  // digitalWrite(LeftDir,LOW);
  // analogWrite(RightEn, con_speed);   //PWM Speed Control
  // analogWrite(LeftEn, con_speed);   //PWM Speed Control 
  // interrupts(); //Enables interrupts
  // computePID();
  // }
  
  // //All off
  // if((digitalRead(LeftSen) == 0) && (digitalRead(LeftMSen) == 0) && (digitalRead(MidSen) == 0) && (digitalRead(RightMSen) == 0) && (digitalRead(RightSen) == 0)){
  //   delay(1000);
  //   if((digitalRead(LeftSen) == 0) && (digitalRead(LeftMSen) == 0) && (digitalRead(MidSen) == 0) && (digitalRead(RightMSen) == 0) && (digitalRead(RightSen) == 0)){
  //   digitalWrite(RightDir, HIGH);
  //   digitalWrite(LeftDir, HIGH);
  //   analogWrite(RightEn, con_speed);
  //   analogWrite(LeftEn, 0);
  //   delay(1000);
  //   digitalWrite(RightDir, HIGH);
  //   digitalWrite(LeftDir, LOW);
  //   analogWrite(RightEn, con_speed);
  //   analogWrite(LeftEn, con_speed);
  //   delay(1000);
  //   }
  // }

  // //sharp turns
  // if((digitalRead(LeftSen) == 1) && (digitalRead(LeftMSen) == 1) && (digitalRead(MidSen) == 1) && (digitalRead(RightMSen) == 0) && (digitalRead(RightSen) == 0)){
  //   digitalWrite(RightDir, HIGH);
  //   digitalWrite(LeftDir, HIGH);
  //   analogWrite(RightEn, con_speed);
  //   analogWrite(LeftEn, 0);
  // }

  // if((digitalRead(LeftSen) == 0) && (digitalRead(LeftMSen) == 0) && (digitalRead(MidSen) == 1) && (digitalRead(RightMSen) == 1) && (digitalRead(RightSen) == 1)){
  //   digitalWrite(RightDir, LOW);
  //   digitalWrite(LeftDir, LOW);
  //   analogWrite(RightEn, 0);
  //   analogWrite(LeftEn, con_speed);
  // }

  // if((digitalRead(LeftSen) == 0) && (digitalRead(LeftMSen) == 0) && (digitalRead(MidSen) == 0) && (digitalRead(RightMSen) == 1) && (digitalRead(RightSen) == 1)){
  //   digitalWrite(RightDir, LOW);
  //   digitalWrite(LeftDir, LOW);
  //   analogWrite(RightEn, 0);
  //   analogWrite(LeftEn, con_speed);
  // }

  // if((digitalRead(LeftSen) == 1) && (digitalRead(LeftMSen) == 1) && (digitalRead(MidSen) == 0) && (digitalRead(RightMSen) == 0) && (digitalRead(RightSen) == 0)){
  //   digitalWrite(RightDir, HIGH);
  //   digitalWrite(LeftDir, HIGH);
  //   analogWrite(RightEn, con_speed);
  //   analogWrite(LeftEn, 0);
  // }
  
}
 
void forward()
{
  digitalWrite(RightDir,HIGH);
  digitalWrite(LeftDir,LOW);
  analogWrite(RightEn, con_speed);   //PWM Speed Control
  analogWrite(LeftEn, con_speed);   //PWM Speed Control 
}

void right(){
  digitalWrite(RightDir,HIGH);
  digitalWrite(LeftDir,LOW);
  analogWrite(RightEn, adjust_speed);   //PWM Speed Control
  analogWrite(LeftEn, con_speed);   //PWM Speed Control
}

void left(){
  digitalWrite(RightDir,HIGH);
  digitalWrite(LeftDir,LOW);
  analogWrite(RightEn, con_speed);   //PWM Speed Control
  analogWrite(LeftEn, adjust_speed);   //PWM Speed Control  
}

void rightturn(){
digitalWrite(RightDir,LOW);
  digitalWrite(LeftDir,LOW);
  analogWrite(RightEn,  0);   //PWM Speed Control
  analogWrite(LeftEn, con_speed);   //PWM Speed Control
}

void leftturn(){
  digitalWrite(RightDir,HIGH);
  digitalWrite(LeftDir,HIGH);
  analogWrite(RightEn, con_speed);   //PWM Speed Control
  analogWrite(LeftEn, 0);   //PWM Speed Control
}

void computePID(){
  int i;
  int powerDiff = 0;
  float Kp,Ki,Kd;
  unsigned int position;
  int derivative, proportional;

  while(1){
    int position =(2*digitalRead(LeftSen) + 1*digitalRead(LeftMSen) + 0*digitalRead(MidSen) + (-1)*digitalRead(RightMSen) + (-2)*digitalRead(RightSen))/(digitalRead(LeftSen)+digitalRead(LeftMSen)+digitalRead(MidSen)+digitalRead(RightMSen)+digitalRead(RightSen));
    proportional = position;
    derivative = proportional - last_proportional;
    integral = integral + proportional;

    Kp = 26;  
    Ki = 0.00015;
    Kd = 5;

    powerDiff = proportional*Kp + integral*Ki + derivative*Kd;
    const int max = 130;

    if(powerDiff>max){
      powerDiff = max;
    }
    else if(powerDiff<-max){
      powerDiff = (-1*max);
      if(powerDiff<0){
        con_speed = max+powerDiff;
        adjust_speed = max;
      }
      else{
        con_speed = max;
        adjust_speed = max-powerDiff;     
      }
    }
  }
}
