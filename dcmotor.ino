int RightEn = 5;
int RightDir = 4;



void setup() {
  //Motor Driver Pin Setup
  pinMode(RightEn,OUTPUT);
  pinMode(RightDir,OUTPUT);  
}

void loop() {
  digitalWrite(RightDir, HIGH);
  analogWrite(RightEn, 0);
}