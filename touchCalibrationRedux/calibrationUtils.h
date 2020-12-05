#include <stdint.h>

#ifndef _ILI9341_t3NH_
#include <ILI9341_t3n.h>
#include <ili9341_t3n_font_Arial.h>
#endif

#ifndef _XPT2046_Touchscreen_h_
#include <XPT2046_Touchscreen.h>
#endif

#define DISP_WIDTH 320
#define DISP_HEIGHT 240
/*
 * Calibration Utilities forward declarations
 */

// helper for calculating average
uint16_t mean(uint16_t* values, uint16_t numValues);

// helper that draws two, intersecting, 1-pixel lines across the screen
void drawFullCross(
    const uint16_t x, 
    const uint16_t y, 
    const uint16_t color,
    ILI9341_t3n* tft
  );

// Helper for drawing calibration screen
// calibPoints is a contiguous array of (x,y) pairs
// indexed by step
void drawCalibrate(ILI9341_t3n* tft, const uint8_t step, const uint16_t* calibPoints);

// Function that sets calibration variables (landscape only, for now)
void doCalibrate(
    ILI9341_t3n* tft,         // Pointer to display tft object
    XPT2046_Touchscreen* ts,  // Pointer to touchscreen sensor object
    uint16_t &minXTS,         // calibration variable
    uint16_t &minYTS,         // calibration variable
    uint16_t &maxXTS,         // calibration variable
    uint16_t &maxYTS,         // calibration variable
    uint8_t  &currentScreen
  );
