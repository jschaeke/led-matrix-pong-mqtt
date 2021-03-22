#ifndef __LEDMATRIXPONG_H
#define __LEDMATRIXPONG_H
#include <FastLED.h>

#include <FastLED_NeoMatrix.h>
#include <LEDMatrix.h>
#include "config.h"

cLEDMatrix<MATRIX_WIDTH, MATRIX_HEIGHT, VERTICAL_ZIGZAG_MATRIX> leds;
#endif // !__LEDMATRIXPONG_H
