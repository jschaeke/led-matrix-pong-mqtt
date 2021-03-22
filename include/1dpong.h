#ifndef __1DPONG_H
#define __1DPONG_H

#include "config.h"
#include <string>

#include <Adafruit_GFX.h>
#include <FastLED_NeoMatrix.h>
#include <LEDMatrix.h>

#define COLOR_ORDER GRB
#define CHIPSET WS2812B

//-----------------------------------------------------
// Ball
//-----------------------------------------------------

typedef void (*OnBallHitBoundCallback)(void *pInstance, int pos);

class Ball
{
public:
  enum class Direction
  {
    Left,
    Right
  };

  Ball(int leftBound, int rightBound,
       void *pCallbackInstance = nullptr, OnBallHitBoundCallback pOnBallHitBoundCallback = nullptr);

  void setBounds(int leftBound, int rightBound);
  void setPos(int pos);
  void setSpeed(int speed);
  void setLastCol(int col);
  void hit(int speed);
  void tick();
  int getPos() const;
  int getLastCol() const;
  bool isMovingToRight() const;
  bool isMovingToLeft() const;
  int getSpeed() const;
  Direction getDirection() const;

private:
  int pos_{NUM_LEDS / 2};
  int speed_{INITIAL_SPEED};
  int leftBound_{0};
  int rightBound_{0};
  int lastCol_{0};
  Direction direction_{Ball::Direction::Right};

  void *pCallbackInstance_{nullptr};
  OnBallHitBoundCallback pOnBallHitBoundCallback_{nullptr};
};

//-----------------------------------------------------
// Player
//-----------------------------------------------------

class Player
{
public:
  enum class BaseStartingPoint
  {
    Left,
    Right
  };

  Player(int basePos, BaseStartingPoint startingPoint);

  void setName(const char *pName);
  bool ballIsInBase(const Ball &ball);
  int basePos() const;
  int numBaseLeds() const;
  int ballPositionInBase(const Ball &ball) const;
  bool ballIsOnLastPixel(const Ball &ball) const;
  const char *getName() const;
  void kill();
  void revive();
  void resetLifes();
  bool isDead() const;
  int lifes() const;

private:
  static const int numBaseLeds_{RANGE};
  int numLifes_{numBaseLeds_};
  bool isDead_{false};
  const int basePos_;
  const BaseStartingPoint baseStartingPoint_;

  std::string name_{"anonymous"};
};

//-----------------------------------------------------
// OneDimensionalPong
//-----------------------------------------------------

typedef void (*OnBallHitBoundCallback)(void *pInstance, int pos);
typedef void (*OnGameStartCallback)();
typedef void (*OnBallHitCallback)(int pos, String direction, int speed);
typedef void (*OnPlayerVictoryCallback)(int playerId);
typedef bool (*onButton1PressedCallback)();
typedef bool (*onButton2PressedCallback)();

class OneDimensionalPong
{
public:
  struct PongCallbacks
  {
    OnGameStartCallback onGameStart;
    OnBallHitCallback onBallHit;
    OnPlayerVictoryCallback onPlayerVictory;
    onButton1PressedCallback isButton1Pressed;
    onButton2PressedCallback isButton2Pressed;
  };

  OneDimensionalPong();
  void setCallbacks(const PongCallbacks &callbacks);
  void setupFastled();
  void init(cLEDMatrix<MATRIX_WIDTH, MATRIX_HEIGHT, VERTICAL_ZIGZAG_MATRIX> *leds);
  void checkButtons();
  void turnOffAllLeds();
  void die();
  void tick();
  void render();

private:
  void drawVerticalLine(int col, int length, int gap);
  void drawBall(int col);
  static void onBallHitBounds(void *pInstance, int pos);

  uint32_t ballColor;

  // Adafruit_NeoPixel pixels_;
  Player player1_;
  Player player2_;
  Ball ball_;
  PongCallbacks callbacks_{0};
  cLEDMatrix<MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_TYPE> *leds;
};

#endif // !__1DPONG_H
