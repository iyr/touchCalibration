/*
 * Test code for calibrating an ILI9341 display
 * with an XPT2046 Touch sensor on a Teensy.
 * (only tested on Teensy 4.1, landscape only)
 * 
 * Configured to use function pointers for function selection
 */
#define CS_PIN  8
#define TFT_DC  9
#define TFT_CS  10
#define TFT_RST 24
// MOSI=11, MISO=12, SCK=13
#define TIRQ_PIN  2

#include <ILI9341_t3n.h>
#include <ili9341_t3n_font_Arial.h>
#include <XPT2046_Touchscreen.h>
#include <SPI.h>

// struct to store state machine variables
#include "smStruct.h"
// functions to set screen calibration variables
#include "calibrationUtils.h"

#define SCREEN_CALIBRATION 0   // start at calibration screen
#define SCREEN_CROSSHAIRDEMO 1 // stuff to do after calibration finished

stateMachine sm;
XPT2046_Touchscreen gts  = XPT2046_Touchscreen(CS_PIN);
ILI9341_t3n gtft         = ILI9341_t3n(TFT_CS, TFT_DC, TFT_RST);

// Array of function pointers, passing only the statemachine
// functions are called by index
void (* SCREENS[2]) (stateMachine*);

void setup() {
  Serial.begin(38400);

  // Initialize State Machine variables
  sm.tft = &gtft;
  sm.ts  = &gts;
  sm.currentScreen = SCREEN_CALIBRATION;

  SCREENS[SCREEN_CALIBRATION]   = doCalibrate;
  SCREENS[SCREEN_CROSSHAIRDEMO] = doCrosshairDemo;
   
  sm.tft->begin();
  sm.tft->setRotation(1);
  sm.tft->fillScreen(ILI9341_BLACK);
  sm.tft->fillScreen(ILI9341_BLUE);
  sm.ts->begin();
  sm.ts->setRotation(1);
  while (!Serial && (millis() <= 1000));
}

void loop() {
  // Call Screen function to draw according function
  (* SCREENS[sm.currentScreen])(&sm);
  delay(1);
}

void doCrosshairDemo(stateMachine* sm) {
  uint16_t posX, posY;
  static uint16_t prvX=0, prvY=0;

  // Get coordinates of touch sensor output,
  // map to screen space
  TS_Point p = sm->ts->getPoint();
  posX = map(p.x, sm->minXTS, sm->maxXTS, 0, DISP_WIDTH);
  posY = map(p.y, sm->minYTS, sm->maxYTS, DISP_HEIGHT, 0);

  // Only redraw if change in value
  if  ( posX != prvX  ||
        posY != prvY  ){
    // draw over old cross to "erase"
    drawFullCross(prvX, prvY, ILI9341_BLACK, sm->tft);
    // draw new cross
    drawFullCross(posX, posY, ILI9341_WHITE, sm->tft);
    // update old cross params
    prvX=posX;
    prvY=posY;
  }
  return;
}

