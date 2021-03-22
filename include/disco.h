#ifndef _DISCO_H
#define _DISCO_H

#include <ColorConverterLib.h>
#include <WS2812FX.h>
#include "config.h"

class Disco
{
public:
  void init();
  void tick();
  void setColor(String hex);
  void setBrightness(uint8_t brightness);
  void setSpeed(uint16_t speed);
  void setMode(uint8_t mode);

private:
  WS2812FX *ws2812fx;
};

#endif // !_DISCO_H
