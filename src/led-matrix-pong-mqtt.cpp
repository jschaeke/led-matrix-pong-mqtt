#include <Arduino.h>

#include "led-matrix-pong-mqtt.h"
#include "message.h"
#include "mqtt.h"
#include "ota.h"
#include "1dpong.h"
#include "disco.h"

OneDimensionalPong _pongGame;
Mqtt _mqtt("jarchitects", "espong", "espong");
Message _message;
Disco _disco;

void onGameStart();
void onBallHit(int pos, String direction, int speed);
void onPlayerVictory(int playerId);
void setup();
void loop();
void onGameStart()
{
   _mqtt.sendGameStartMsg();
}

void onBallHit(int pos, String direction, int speed)
{
   _mqtt.sendBallHitMsg(pos, direction, speed);
}

void onPlayerVictory(int playerId)
{
   _mqtt.sendPlayerVictoryMsg(playerId);
}

bool isButton1Pressed()
{
   return _mqtt.isButton1Pressed();
}

bool isButton2Pressed()
{
   return _mqtt.isButton2Pressed();
}

void setup()
{
   Serial.begin(115200);
   _message.init(&leds);

   setupOta();
   _mqtt.init();
   _disco.init();

   OneDimensionalPong::PongCallbacks cb{0};
   cb.onGameStart = onGameStart;
   cb.onBallHit = onBallHit;
   cb.onPlayerVictory = onPlayerVictory;
   cb.isButton1Pressed = isButton1Pressed;
   cb.isButton2Pressed = isButton2Pressed;

   _pongGame.setCallbacks(cb);
   _pongGame.init(&leds);
}

void loop()
{
   if (!_message.displayingText())
   {
      // Message instructions
      if (_mqtt.isMessageReceived())
      {
         _message.displayMessage(_mqtt.getMessage());
      }
      if (_mqtt.isBrightnessReceived())
      {  
         uint8_t brightness = atoi(_mqtt.getMessage());
         _message.setBrightness(brightness);
         _disco.setBrightness(brightness);
      }
      if (_mqtt.isFramerateReceived())
      {
         _message.setFrameRate(atoi(_mqtt.getMessage()));
      }
      if (_mqtt.isScrollDirectionReceived())
      {
         _message.setScrollDirection(strtol(_mqtt.getMessage(), NULL, 0));
      }
      if (_mqtt.isTextColorReceived())
      {
         char* color = _mqtt.getMessage();
         _message.setColor(color);
         _disco.setColor(color);
      }
      if (_mqtt.isTextDirectionReceived())
      {
         _message.setTextDirection(strtol(_mqtt.getMessage(), NULL, 0));
      }
      
      // Check game is paused
      if (!_mqtt.isGamePaused())
      {
         _pongGame.tick();
      }
      if (!_mqtt.isDiscoPaused()){
         if (_mqtt.isModeReceived())
         {
            _disco.setMode(atoi(_mqtt.getMessage()));
         }
         if (_mqtt.isDiscoSpeedReceived()){
            _disco.setSpeed(strtol(_mqtt.getMessage(), NULL, 0));
         }
         _disco.tick();
      }
   }

   _mqtt.tick();
   ArduinoOTA.handle();
}