#include "message.h"
#include "jarchitects2.c"

#define LED_RED_HIGH (31 << 11)
#define LED_RED_VERYLOW (3 << 11)
#define LED_GREEN_VERYLOW (1 << 5)
#define LED_BLUE_VERYLOW 3
#define LED_WHITE_VERYLOW (LED_RED_VERYLOW + LED_GREEN_VERYLOW + LED_BLUE_VERYLOW)
Message::Message(void)
{
}

void Message::init(cLEDMatrix<MATRIX_WIDTH, MATRIX_HEIGHT, VERTICAL_ZIGZAG_MATRIX> *leds)
{
  matrix = new FastLED_NeoMatrix(ledsCRGB, MATRIX_WIDTH, 8,
                                                    NEO_MATRIX_TOP + NEO_MATRIX_LEFT +
                                                        NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG);
  FastLED.addLeds<CHIPSET, LED_DATA_PIN, COLOR_ORDER>(ledsCRGB, NUM_LEDS);
  matrix->drawBitmap(0, 0, (const uint8_t *)jarchitects2, MATRIX_WIDTH, 8, LED_WHITE_VERYLOW, LED_RED_HIGH);
  matrix->setBrightness(LED_BRIGHTNESS);
  FastLED.show();
  FastLED.addLeds<CHIPSET, LED_DATA_PIN, COLOR_ORDER>(leds->operator[](0), leds->Size());

  ScrollingMsg.SetFont(MatriseFontData);
  ScrollingMsg.Init(leds, leds->Width(), ScrollingMsg.FontHeight() + 1, 0, 0);
  ScrollingMsg.SetTextColrOptions(COLR_RGB , 100, 100 , 100);
}

bool Message::displayingText()
{
  if (ScrollingMsg.UpdateText() != -1)
  {
    FastLED.show();
  }
  return ScrollingMsg.UpdateText() == 0;
}

void Message::displayMessage(char * message)
{
  ScrollingMsg.SetText((unsigned char*)message, strlen((const char*)message)+1);
}

void Message::setFrameRate(uint8_t rate)
{
  ScrollingMsg.SetFrameRate(rate);
}

void Message::setBrightness(uint8_t brightness)
{
  matrix->setBrightness(brightness);
}

void Message::setScrollDirection(uint16_t options)
{
  ScrollingMsg.SetScrollDirection(options);
}

void Message::setTextDirection(uint16_t options)
{
  ScrollingMsg.SetTextDirection(options);
}

void Message::setColor(String hex)
{
  uint8_t red, green, blue;
  ColorConverter::HexToRgb(hex, red, green, blue);
  ScrollingMsg.SetTextColrOptions(COLR_RGB , red, green , blue);
}