
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>

// physical dimensions of the tft display (# of pixels)
#define TFT_WIDTH  480
#define TFT_HEIGHT 320

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

MCUFRIEND_kbv tft;
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
// different than SD

void setup() {
  init();
  Serial.begin(9600);
  // tft display initialization
  uint16_t ID = tft.readID();
  tft.begin(ID);
  tft.fillScreen(TFT_WHITE);
  tft.setRotation(1);
}

int pointX, pointY; bool first = true;

void processTouchScreen() {
  TSPoint touch = ts.getPoint();
  pinMode(YP, OUTPUT); 
  pinMode(XM, OUTPUT); 
  if (touch.z < MINPRESSURE || touch.z > MAXPRESSURE) {
	return;
  }
  int16_t screen_x = map(touch.y, TS_MINX, TS_MAXX, TFT_WIDTH-1, 0);
  int16_t screen_y = map(touch.x, TS_MINY, TS_MAXY, TFT_HEIGHT-1, 0);
  if (first) {
    tft.drawPixel(screen_x, screen_y, TFT_BLACK);
    first = false;
  } else {
    tft.drawLine(pointX, pointY, screen_x, screen_y, TFT_BLACK);
  }
  pointX = screen_x; pointY = screen_y;
}

// void processTouchScreen() {
//   TSPoint touch = ts.getPoint();
//   pinMode(YP, OUTPUT); 
//   pinMode(XM, OUTPUT); 
//   if (touch.z < MINPRESSURE || touch.z > MAXPRESSURE) {
// 	return;
//   }
//   int16_t screen_x = map(touch.y, TS_MINX, TS_MAXX, TFT_WIDTH-1, 0);
//   int16_t screen_y = map(touch.x, TS_MINY, TS_MAXY, TFT_HEIGHT-1, 0);

//   tft.fillRect(screen_x - 1,screen_y - 1, CURSOR_SIZE, CURSOR_SIZE, TFT_BLACK);

// }
int main() {
  setup();
  while (true) {
    processTouchScreen();
  }
  Serial.end();
  return 0;
}