
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

uint16_t pointsX[MAX_ARRAYSIZE] = { };
uint16_t pointsY[MAX_ARRAYSIZE] = { };
uint16_t pointIndex = 0;

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

uint16_t uint16_from_serial3() {
  uint16_t num = 0;
  num = num | ((uint32_t)Serial3.read()) << 0;
  num = num | ((uint32_t)Serial3.read()) << 8;
  return num;
}

void receiveArray() {
  bool endRead = false;
  while (!endRead) {
    pointsX[pointIndex] = uint16_from_serial3();
    pointsY[pointIndex] = uint16_from_serial3();

    if (i > 0 && pointsX[pointIndex] == 0 && pointsY[pointIndex] == 0) {
      if (pointsX[pointIndex - 1] == 0 && pointsY[pointIndex - 1] == 0) {
        endRead = true;
      }
    } 
    pointIndex ++;
    if (pointIndex == MAX_ARRAYSIZE) {
      endRead = true;
    } 
  } 
}


void drawOnScreen() {
  receiveArray();
  for (uint16__t i = 2; i < pointIndex; i++) {
    if (pointsX[i] != 0 && pointsY[i] != 0) {
      tft.drawLine(pointsX[i - 1], pointsY[i - 1], pointsX[i], pointsY[i], TFT_BLACK);
    } else {
      i++;
    }
  }
}

int main() {
  setup();
  while (true) {
    drawOnScreen();
  }
  Serial.end();
  Serial3.end();
  return 0;
}