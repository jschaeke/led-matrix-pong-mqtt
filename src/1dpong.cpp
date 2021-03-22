#include "1dpong.h"
#include <FontMatrise.h>
//-----------------------------------------------------
// Ball
//-----------------------------------------------------

Ball::Ball(int leftBound, int rightBound, void *pCallbackInstance,
           OnBallHitBoundCallback pOnBallHitBoundCallback) : pCallbackInstance_(pCallbackInstance),
                                                             pOnBallHitBoundCallback_(pOnBallHitBoundCallback)
{

  setPos(NUM_LEDS / 2);
  setBounds(leftBound, rightBound);
}

void Ball::setPos(int pos)
{
  pos_ = pos;
}

void Ball::setSpeed(int speed)
{
  speed_ = speed;
}

void Ball::setLastCol(int col)
{
  lastCol_ = col;
}

void Ball::hit(int speed)
{
  direction_ = direction_ == Direction::Right ? Direction::Left : Direction::Right;
  speed_ += speed;
}

void Ball::tick()
{
  //Serial.println(speed_);
  static int lastTime = millis();
  int ledsPerHit = 8;
  // Speed up for middle columns
  boolean inMiddle = (pos_ > 128) && (pos_ < 640);
  if ((((unsigned)(millis() - lastTime))) < ((unsigned)(5000 / (1 + speed_))) && !inMiddle)
  {
    return;
  }

  lastTime = millis();

  int newPos = pos_ + (direction_ == Direction::Right ? ledsPerHit : -1 * ledsPerHit);

  if (newPos < leftBound_ || newPos == rightBound_)
  {
    if (pOnBallHitBoundCallback_)
      pOnBallHitBoundCallback_(pCallbackInstance_, pos_);

    return;
  }

  pos_ = newPos;
}

int Ball::getPos() const
{
  return pos_;
}

int Ball::getLastCol() const
{
  return lastCol_;
}

int Ball::getSpeed() const
{
  return speed_;
}

void Ball::setBounds(int leftBound, int rightBound)
{
  leftBound_ = leftBound;
  rightBound_ = rightBound;
}

bool Ball::isMovingToRight() const
{
  return direction_ == Direction::Right;
}

bool Ball::isMovingToLeft() const
{
  return direction_ == Direction::Left;
}

Ball::Direction Ball::getDirection() const
{
  return direction_;
}

//-----------------------------------------------------
// Player
//-----------------------------------------------------

Player::Player(int basePos, BaseStartingPoint baseStartingPoint) : basePos_(basePos), baseStartingPoint_(baseStartingPoint)
{
}

void Player::setName(const char *pName)
{
  name_ = pName;
}

bool Player::ballIsInBase(const Ball &ball)
{
  return ballPositionInBase(ball) != 0;
}

int Player::ballPositionInBase(const Ball &ball) const
{
  if (baseStartingPoint_ == BaseStartingPoint::Left)
  {
    if (ball.getPos() < basePos_ + numBaseLeds_ + 40)
      return basePos_ + numBaseLeds_ - ball.getPos();
  }
  else if (baseStartingPoint_ == BaseStartingPoint::Right)
  {
    if (ball.getPos() >= basePos_ - 40)
      return 1 + ball.getPos() - basePos_;
  }

  return 0;
}

int Player::basePos() const
{
  return basePos_;
}

int Player::numBaseLeds() const
{
  return numBaseLeds_;
}

bool Player::ballIsOnLastPixel(const Ball &ball) const
{
  return ball.getPos() == basePos_;
}

void Player::kill()
{
  numLifes_--;
  isDead_ = true;
}

bool Player::isDead() const
{
  return isDead_;
}

void Player::revive()
{
  isDead_ = false;
}

const char *Player::getName() const
{
  return name_.c_str();
}

int Player::lifes() const
{
  return numLifes_;
}

void Player::resetLifes()
{
  numLifes_ = numBaseLeds_;
}

//-----------------------------------------------------
// OneDimensionalPong
//-----------------------------------------------------

OneDimensionalPong::OneDimensionalPong() : //pixels_(NUM_LEDS, LED_DATA_PIN, NEO_GRB + NEO_KHZ800),
                                           player1_(0, Player::BaseStartingPoint::Left), player2_(NUM_LEDS - RANGE, Player::BaseStartingPoint::Right),
                                           ball_(player1_.basePos(), player2_.basePos() + player2_.numBaseLeds(), this, onBallHitBounds)
{

  player1_.setName("Player 1");
  player2_.setName("Player 2");
}

void OneDimensionalPong::setupFastled()
{
  // FastLED.addLeds<CHIPSET, LED_DATA_PIN, COLOR_ORDER>(leds[0], leds.Size());
  // FastLED.setBrightness(LED_BRIGHTNESS);
  // FastLED.clear(true);
  // FastLED.show();
}

void OneDimensionalPong::init(cLEDMatrix<MATRIX_WIDTH, MATRIX_HEIGHT, VERTICAL_ZIGZAG_MATRIX> *leds)
{
  pinMode(BUTTON_1_PIN, INPUT_PULLUP);
  pinMode(BUTTON_2_PIN, INPUT_PULLUP);

  this->leds = leds;

  setupFastled();
  turnOffAllLeds();

  if (callbacks_.onGameStart)
    callbacks_.onGameStart();
}

void OneDimensionalPong::setCallbacks(const PongCallbacks &callbacks)
{
  callbacks_ = callbacks;
}

void OneDimensionalPong::checkButtons()
{
  static int lastB1 = 0;
  static int lastB2 = 0;
  int b1 = digitalRead(BUTTON_1_PIN);
  int b2 = digitalRead(BUTTON_2_PIN);

  if (callbacks_.isButton1Pressed())
    b1 = LOW;
  if (callbacks_.isButton2Pressed())
    b2 = LOW;

  if (player1_.ballIsInBase(ball_) && !lastB1)
  {
    if (b1 && ball_.isMovingToLeft())
    {
      ball_.hit(10);

      if (callbacks_.onBallHit)
      {
        const char *pDirection = ball_.getDirection() == Ball::Direction::Right ? "right" : "left";
        callbacks_.onBallHit(ball_.getPos(), pDirection, ball_.getSpeed());
      }
    }
  }
  else if (player2_.ballIsInBase(ball_) && !lastB2)
  {
    if (b2 && ball_.isMovingToRight())
    {
      ball_.hit(10);

      if (callbacks_.onBallHit)
      {
        const char *pDirection = ball_.getDirection() == Ball::Direction::Right ? "right" : "left";
        callbacks_.onBallHit(ball_.getPos(), pDirection, ball_.getSpeed());
      }
    }
  }

  lastB1 = b1;
  lastB2 = b2;
}

void OneDimensionalPong::turnOffAllLeds()
{
  FastLED.clear();
  FastLED.show();
  //delay(50);
}

void OneDimensionalPong::die()
{
  turnOffAllLeds();

  // Blink
  for (int i = 0; i < 3; i++)
  {
    leds->operator()(ball_.getPos()) = CRGB::Red;
    FastLED.show();
    //delay(500);
    turnOffAllLeds();
    //delay(500);
  }

  ball_.setPos(NUM_LEDS / 2);

  if (player1_.isDead())
    player1_.revive();

  if (player2_.isDead())
    player2_.revive();
}

void OneDimensionalPong::tick()
{
  checkButtons();
  ball_.tick();

  if (player1_.isDead())
  {
    die();

    if (player1_.lifes() == player1_.numBaseLeds() && player2_.lifes() == player2_.numBaseLeds())
    {
      if (callbacks_.onGameStart)
      {
        callbacks_.onGameStart();
      }
    }
  }
  else if (player2_.isDead())
  {
    die();

    if (player1_.lifes() == player1_.numBaseLeds() && player2_.lifes() == player2_.numBaseLeds())
    {
      if (callbacks_.onGameStart)
      {
        callbacks_.onGameStart();
      }
    }
  }
  render();
}

void OneDimensionalPong::drawBall(int col)
{
  leds->DrawFilledCircle(col, 4, 2, CRGB::Yellow);
}

void OneDimensionalPong::render()
{
  FastLED.clear();
  // players
  for (int i = 0; i < player1_.numBaseLeds(); i++)
  {
    int pos = i + player1_.basePos();

    if (pos < player1_.lifes())
      leds->operator()(pos) = CRGB::Blue;
    else
      leds->operator()(pos) = CRGB::Red;
  }
  leds->operator()(player1_.basePos() + 32) = CRGB::Blue;
  leds->operator()(player1_.basePos() + 39) = CRGB::Blue;

  leds->operator()(player2_.basePos() - 32) = CRGB::Blue;
  leds->operator()(player2_.basePos() - 25) = CRGB::Blue;

  for (int i = 0; i < player2_.numBaseLeds(); i++)
  {
    int pos = i + player2_.basePos();

    if (pos - player2_.basePos() >= player2_.numBaseLeds() - player2_.lifes())
      leds->operator()(pos) = CRGB::Blue;
    else
      leds->operator()(pos) = CRGB::Red;
  }

  int col = (ball_.getPos()) / 8;
  if (ball_.getLastCol() == col)
  {
    return;
  }
  // ball
  if (ball_.getPos() >= 8 && ball_.getPos() < 760)
  {

    drawBall(col);
  }
  ball_.setLastCol(col);
  FastLED.show();
}

void OneDimensionalPong::onBallHitBounds(void *pInstance, int pos)
{
  OneDimensionalPong *pThis = static_cast<OneDimensionalPong *>(pInstance);

  if (pThis->player1_.ballIsInBase(pThis->ball_))
  {
    pThis->ball_.hit(0);
    pThis->player1_.kill();
    if (pThis->player1_.lifes() == 0)
    {
      if (pThis->callbacks_.onPlayerVictory)
        pThis->callbacks_.onPlayerVictory(2);
    }
  }
  else if (pThis->player2_.ballIsInBase(pThis->ball_))
  {
    pThis->ball_.hit(0);
    pThis->player2_.kill();
    if (pThis->player2_.lifes() == 0)
    {
      if (pThis->callbacks_.onPlayerVictory)
        pThis->callbacks_.onPlayerVictory(1);
    }
  }

  if (pThis->player1_.lifes() == 0 || pThis->player2_.lifes() == 0)
  {
    pThis->player1_.resetLifes();
    pThis->player2_.resetLifes();
    pThis->ball_.setSpeed(INITIAL_SPEED);
  }
}
