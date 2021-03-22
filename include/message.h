#ifndef _MESSAGE_H
#define _MESSAGE_H

#include <FastLED.h>

#include <FastLED_NeoMatrix.h>
#include <LEDMatrix.h>
#include <LEDText.h>
#include <FontMatrise.h>
#include <ColorConverterLib.h>
#include "config.h"

#define COLOR_ORDER GRB
#define CHIPSET WS2812B

class Message
{
public:
  Message();
  void init(cLEDMatrix<MATRIX_WIDTH, MATRIX_HEIGHT, VERTICAL_ZIGZAG_MATRIX> *leds);
  void displayMessage(char * message);
  void setBrightness(uint8_t brightness);
  void setFrameRate(uint8_t rate);
  void setScrollDirection(uint16_t options);
  void setTextDirection(uint16_t options);
  void setColor(String hex);
  bool displayingText();

private:
  CRGB ledsCRGB[NUM_LEDS];
  FastLED_NeoMatrix *matrix;
 // WS2812FX *ws2812fx;
  cLEDText ScrollingMsg;
};

#endif // !_MESSAGE_H
