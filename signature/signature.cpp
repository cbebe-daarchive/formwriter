
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>

// physical dimensions of the tft display (# of pixels)
#define TFT_WIDTH  480
#define TFT_HEIGHT 320
#define FORM_WIDTH 360
#define FORM_HEIGHT 240

// touch screen pins, obtained from the documentation
#define YP A3 // must be an analog pin, use "An" notation!
#define XM A2 // must be an analog pin, use "An" notation!
#define YM 9  // can be a digital pin
#define XP 8  // can be a digital pin

// calibration data for the touch screen, obtained from documentation
// the minimum/maximum possible readings from the touch point
#define TS_MINX 100
#define TS_MINY 120
#define TS_MAXX 940
#define TS_MAXY 920
// thresholds to determine if there was a touch
#define MINPRESSURE   10
#define MAXPRESSURE 1000

#define CURSOR_SIZE 3

#define MAX_ARRAYSIZE 500

MCUFRIEND_kbv tft;
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

uint16_t pointX, pointY; bool penLift = true;
uint16_t pointsX[MAX_ARRAYSIZE] = { };
uint16_t pointsY[MAX_ARRAYSIZE] = { };
uint16_t pointIndex = 0;
bool dataSent = false;

void writeVertical(char text[], int X, int Y, int length) {
  for (int8_t i = 0; i < length; i++) {
    tft.setCursor(X, Y + (16 * i));
    tft.print(text[i]);
  }
}

void setupFields() {
  tft.drawRect(30, 40, FORM_WIDTH, FORM_HEIGHT, TFT_BLACK);
  tft.drawRect(TFT_WIDTH - 60, 0, 60, 320, TFT_BLACK);
  tft.drawRect(TFT_WIDTH - 60, TFT_HEIGHT/2, 60, 320, TFT_BLACK);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  char reset[] = "RESET";
  char send[] = "SEND";
  writeVertical(reset, TFT_WIDTH - 35, 45, 5);
  writeVertical(send, TFT_WIDTH - 35, TFT_HEIGHT/2 + 45, 4);
}

void setup() {
  init();
  Serial.begin(9600);
  Serial3.begin(9600);
  // tft display initialization
  uint16_t ID = tft.readID();
  tft.begin(ID);
  tft.fillScreen(TFT_WHITE);
  tft.setRotation(1);
  setupFields();
}

void clearOutputField() {
  tft.fillRect(30, TFT_HEIGHT - 30, 330, 20, TFT_WHITE);
  tft.setCursor(30, TFT_HEIGHT - 30);  
}

void fieldReset() {
  tft.fillRect(31, 41, FORM_WIDTH - 2, FORM_HEIGHT - 2, TFT_WHITE);
  penLift = false;
  for (uint16_t i = 0; i < MAX_ARRAYSIZE; i++) {
    pointsX[i] = 0;
    pointsY[i] = 0;
  }
  pointIndex = 0;
}

void serialFeedback(uint32_t delta) {
  clearOutputField();
  tft.print("Array size: "); tft.print(pointIndex);
  delay(1500);
  clearOutputField();
  tft.print("Time to submit: "); tft.print(delta);
  tft.println(" ms");

}

void uint16_to_serial3(uint16_t num) {
  Serial3.write((char)(num >> 0));
  Serial3.write((char)(num >> 8));
}

void fieldSubmit() {
  clearOutputField();
  tft.println("Sending to Serial...");
  uint32_t timeA = millis();
  for (uint16_t i = 0; i < pointIndex; i++) {
    uint16_to_serial3(pointsX[pointIndex]);
    uint16_to_serial3(pointsY[pointIndex]);
  }
  serialFeedback(millis() - timeA);
  dataSent = true; 
  delay(200);
}

void overflowHandle() {
  clearOutputField();
  tft.println("Overflow! Please try again.");
  delay(500);
  fieldReset();
  clearOutputField();
}

void drawSignature(int16_t screen_x, int16_t screen_y) {
  if (penLift) {
    tft.drawPixel(screen_x, screen_y, TFT_BLACK);
    penLift = false;
    pointsX[pointIndex] = 0;
    pointsY[pointIndex] = 0;
    pointIndex++;  
    
  } else {
    tft.drawLine(pointX, pointY, screen_x, screen_y, TFT_BLACK);
  }
  pointsX[pointIndex] = screen_x;
  pointsY[pointIndex] = screen_y;
  pointX = screen_x; pointY = screen_y;
  pointIndex++;
  if (pointIndex >= MAX_ARRAYSIZE) {
    overflowHandle();
  }
}

bool processTouchScreen() {
  TSPoint touch = ts.getPoint();
  pinMode(YP, OUTPUT); 
  pinMode(XM, OUTPUT); 

  if (touch.z < MINPRESSURE || touch.z > MAXPRESSURE) {
    return false;
  }

  int16_t screen_x = map(touch.y, TS_MINX, TS_MAXX, TFT_WIDTH-1, 0);
  int16_t screen_y = map(touch.x, TS_MINY, TS_MAXY, TFT_HEIGHT-1, 0);
  if (screen_x > 30 && screen_x < 390 && screen_y > 40 && screen_y < 280) {
    if (dataSent) {
      clearOutputField();
      dataSent = false;
    }
    drawSignature(screen_x, screen_y);
  } else if (screen_x > TFT_WIDTH - 60) {
    if (screen_y > TFT_HEIGHT/2) {
      fieldSubmit();
    }
    fieldReset();
  }
  return true;
}

void sign() {
  int delayCount = 0;
  while (!processTouchScreen() && delayCount < 50) {
    delay(1); delayCount++;
  }
  if (delayCount == 50) {
  	penLift = true;	
  }  
  delay(15);
}

int main() {
  setup();
  while (true) {
    sign();
  }
  Serial.end();
  Serial3.end();
  return 0;
}