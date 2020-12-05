
/*
 * Test code for calibrating an ILI9341 display
 * with an XPT2046 Touch sensor on a Teensy.
 * (only tested on Teensy 4.1, landscape only)
 */

#include <ILI9341_t3n.h>
#include <ili9341_t3n_font_Arial.h>
#include <XPT2046_Touchscreen.h>
#include <SPI.h>

#include "calibrationUtils.h"

#define CS_PIN  8
#define TFT_DC  9
#define TFT_CS  10
#define TFT_RST 24
// MOSI=11, MISO=12, SCK=13
XPT2046_Touchscreen ts(CS_PIN);
#define TIRQ_PIN  2
ILI9341_t3n tft = ILI9341_t3n(TFT_CS, TFT_DC, TFT_RST);

#define SCREEN_CALIBRATION 0   // start at calibration screen
#define SCREEN_CROSSHAIRDEMO 1 // stuff to do after calibration finished
uint8_t currentScreen = SCREEN_CALIBRATION;

// Calibration variables
// for mapping touch-sensor coordinates to 
// screen-space coordinates and vice/versa
uint16_t minXTS, minYTS, maxXTS, maxYTS;

void setup() {
  Serial.begin(38400);
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  ts.begin();
  ts.setRotation(1);
  while (!Serial && (millis() <= 1000));
}

void loop() {
  switch(currentScreen) {
    case SCREEN_CALIBRATION: 
      doCalibrate(&tft, &ts, minXTS, minYTS, maxXTS, maxYTS, currentScreen); 
      break;
    case SCREEN_CROSSHAIRDEMO: 
      if (ts.touched()) doCrosshairDemo(); 
      break;
  }

  delay(1);
}

void doCrosshairDemo(void) {
  uint16_t posX, posY;
  static uint16_t prvX=0, prvY=0;

  // Get coordinates of touch sensor output,
  // map to screen space
  TS_Point p = ts.getPoint();
  posX = map(p.x, minXTS, maxXTS, 0, DISP_WIDTH);
  posY = map(p.y, minYTS, maxYTS, DISP_HEIGHT, 0);

  // Only redraw if change in value
  if  ( posX != prvX  ||
        posY != prvY  ){
    // draw over old cross to "erase"
    drawFullCross(prvX, prvY, ILI9341_BLACK, &tft);
    // draw new cross
    drawFullCross(posX, posY, ILI9341_WHITE, &tft);
    // update old cross params
    prvX=posX;
    prvY=posY;
  }
  return;
}

