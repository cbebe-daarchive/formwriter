
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
  Serial.print("X: {");
  for (uint16_t i = 0; i < pointIndex; i++) {
    Serial.print(pointsX[i]); Serial.print(", ");
    uint16_to_serial3(pointsX[pointIndex]);
  }
  Serial.print("} Y: ");
  for (uint16_t i = 0; i < pointIndex; i++) {
    Serial.print(pointsY[i]); Serial.print(", ");
    uint16_to_serial3(pointsY[pointIndex]);
  }
  Serial.println("}");
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
uint16_t X[] = {0, 89, 89, 88, 89, 92, 95, 100, 104, 111, 117, 123, 125, 128, 127, 125, 122,
 120, 119, 118, 118, 117, 119, 119, 120, 120, 120, 120, 119, 119, 119, 119, 119,
 119, 120, 122, 128, 133, 138, 141, 140, 135, 127, 119, 115, 112, 111, 0, 158, 1
57, 156, 156, 155, 156, 157, 163, 170, 178, 184, 187, 186, 181, 173, 162, 156, 1
54, 156, 158, 162, 169, 180, 188, 0, 198, 196, 196, 199, 206, 215, 222, 228, 230
, 229, 223, 216, 208, 200, 194, 191, 192, 198, 208, 217, 223, 0, 242, 242, 242, 
243, 246, 247, 250, 253, 254, 255, 255, 0, 280, 279, 277, 276, 276, 276, 278, 28
0, 282, 283, 283, 284, 284, 284, 0, 246, 247, 249, 255, 263, 275, 283, 286, 287,
 0, 302, 302, 301, 299, 296, 294, 295, 300, 304, 310, 314, 317, 317, 316, 311, 3
07, 303, 300, 299, 299, 299, 299, 299, 299, 300, 300, 302, 303, 306, 307, 310, 3
11, 314, 316, 320, 322, 323, 326, 326, 326, 326, 0, 336, 335, 333, 331, 329, 329
, 331, 336, 340, 343, 347, 348, 348, 349, 350, 351, 351, 350, 350, 350, 351, 355
, 361, 365, 368, 371, 372, 368, 369, }

uint16_t Y[] = {0, 113, 114, 115, 118, 123, 128, 136, 
141, 146, 147, 148, 147, 143, 136, 130, 125, 123, 123, 123, 123, 122, 122, 121, 
121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 122, 124, 129, 136, 146, 158, 
170, 181, 191, 200, 205, 207, 208, 0, 177, 176, 175, 175, 175, 175, 175, 175, 17
5, 174, 171, 167, 162, 157, 155, 158, 164, 170, 178, 186, 193, 195, 193, 190, 0,
 177, 174, 175, 175, 176, 176, 174, 170, 165, 160, 155, 153, 154, 158, 165, 173,
 180, 184, 185, 184, 182, 0, 136, 135, 136, 138, 144, 153, 166, 180, 192, 197, 1
99, 0, 132, 130, 130, 130, 131, 134, 139, 148, 162, 176, 188, 193, 194, 194, 0, 
170, 168, 167, 167, 168, 167, 165, 164, 164, 0, 170, 167, 166, 166, 169, 174, 17
9, 185, 188, 189, 187, 182, 174, 169, 164, 162, 164, 166, 168, 168, 168, 168, 16
8, 168, 167, 166, 165, 164, 163, 162, 161, 160, 160, 162, 166, 172, 179, 185, 18
9, 190, 190, 0, 165, 161, 160, 162, 165, 169, 172, 177, 182, 185, 186, 186, 185,
 182, 176, 170, 167, 166, 166, 167, 169, 174, 178, 179, 177, 174, 167, 160, 157}  
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
