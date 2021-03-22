#ifndef _MQTT_H
#define _MQTT_H

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

class Mqtt
{
public:
  Mqtt(String location, String name, String role);
  void init();
  bool isButton1Pressed();
  bool isButton2Pressed();
  bool isMessageReceived();
  bool isBrightnessReceived();
  bool isFramerateReceived();
  bool isScrollDirectionReceived();
  bool isTextDirectionReceived();
  bool isTextColorReceived();
  bool isModeReceived();
  bool isGamePaused();
  bool isDiscoPaused();
  bool isDiscoSpeedReceived();
  char * getMessage();
  void tick();
  void sendGameStartMsg();
  void sendBallHitMsg(int pos, String direction, int speed);
  void sendPlayerVictoryMsg(int playerId);

private:
  void reconnect();
  void callback(char *topic, byte *payload, unsigned int length);
  void sendDiscoveryMsg();
  void sendOnlineMsg();

  WiFiClient wifiClient_;
  PubSubClient mqttClient_;
  String prefix_;
  String role_;
  bool remoteButton1;
  bool remoteButton2;
  bool messageReceived;
  bool pauseGame;
  bool pauseDisco;
  bool brightnessReceived;
  bool framerateReceived;
  bool scrollDirectionReceived;
  bool textDirectionReceived; 
  bool textColorReceived;
  bool modeReceived;
  bool discoSpeedReceived;
  char* message;
};

#endif // !_MQTT_H
