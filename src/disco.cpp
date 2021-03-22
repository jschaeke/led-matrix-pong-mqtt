#include "disco.h"

void Disco::init()
{
  ws2812fx = new WS2812FX(NUM_LEDS, LED_DATA_PIN, NEO_GRB + NEO_KHZ800);
  ws2812fx->init();
  ws2812fx->setBrightness(LED_BRIGHTNESS);
  ws2812fx->setSpeed(2);
  ws2812fx->setMode(FX_MODE_FIREWORKS_RANDOM);
  ws2812fx->start();
}

void Disco::tick()
{
  ws2812fx->service();
}

void Disco::setBrightness(uint8_t brightness)
{
  ws2812fx->setBrightness(brightness);
}

void Disco::setMode(uint8_t mode)
{
  ws2812fx->setMode(mode);
}

void Disco::setSpeed(uint16_t speed)
{
  ws2812fx->setSpeed(speed);
}

void Disco::setColor(String hex)
{
  uint8_t red, green, blue;
  ColorConverter::HexToRgb(hex, red, green, blue);
  ws2812fx->setColor(red, green, blue);
}