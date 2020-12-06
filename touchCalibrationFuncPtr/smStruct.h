
// Defines a stateMachine whose address 
// can be passed around to functions
// instead of many disparate function arguments
#ifndef _SMstruct_
struct stateMachine {
  ILI9341_t3n *tft;
  XPT2046_Touchscreen *ts;

  uint8_t currentScreen = 0;

  // Calibration variables
  // for mapping touch-sensor coordinates to 
  // screen-space coordinates and vice/versa
  uint16_t minXTS, minYTS, maxXTS, maxYTS;
};
#endif
#define _SMstruct_

