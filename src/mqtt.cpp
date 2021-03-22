#include <ArduinoJson.h>
#include "config.h"
#include "mqtt.h"

// enum instruction
// {
//   BUTTON1 = 1,
//   BUTTON2,
//   MESSAGE,
//   LOGO,
//   PONG_PAUSE,
//   PONG_RESUME
// };

Mqtt::Mqtt(String location, String name, String role) : mqttClient_(wifiClient_)
{
  prefix_ = location + "/" + name + "/";
  role_ = role;
  remoteButton1 = false;
  remoteButton2 = false;
  messageReceived = false;
  pauseGame = false;
  pauseDisco = true;
  brightnessReceived = false;
  framerateReceived = false;
  scrollDirectionReceived = false;
  textDirectionReceived = false;
  textColorReceived = false;
  modeReceived = false;
  discoSpeedReceived = false;
}

void Mqtt::reconnect()
{
  // Loop until we're reconnected
  while (!mqttClient_.connected())
  {
    if (mqttClient_.connect((role_ + "/online").c_str(), mqtt_user, mqtt_password, "matrix/status/online", 1, true, "false"))
    {}
    else
    {
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void Mqtt::callback(char *topic, byte *payload, unsigned int length)
{
  payload[length] = '\0'; // To clear out previous payloads
  message = (char *)payload;
  if (strcmp(topic, "matrix/in/button1") == 0)
  {
    remoteButton1 = true;
  }
  else if (strcmp(topic, "matrix/in/button2") == 0)
  {
    remoteButton2 = true;
  }
  else if (strcmp(topic, "matrix/in/message") == 0)
  {
    messageReceived = true;
  }
  else if (strcmp(topic, "matrix/in/pongpause") == 0)
  {
    String status = String((char *)payload);
    if (status == "true")
    {
      pauseGame = true;
    }
    else if (status == "false")
    {
      pauseGame = false;
    }
  }
  else if (strcmp(topic, "matrix/in/discopause") == 0)
  {
    String status = String((char *)payload);
    if (status == "true")
    {
      pauseDisco = true;
    }
    else if (status == "false")
    {
      pauseDisco = false;
    }
  }
  else if (strcmp(topic, "matrix/in/discospeed") == 0)
  {
    discoSpeedReceived = true;
  }
  else if (strcmp(topic, "matrix/in/brightness") == 0)
  {
    brightnessReceived = true;
  }
  else if (strcmp(topic, "matrix/in/framerate") == 0)
  {
    framerateReceived = true;
  }
  else if (strcmp(topic, "matrix/in/scrolldirection") == 0)
  {
    scrollDirectionReceived = true;
  }
  else if (strcmp(topic, "matrix/in/textdirection") == 0)
  {
    textDirectionReceived = true;
  }
  else if (strcmp(topic, "matrix/in/textcolor") == 0)
  {
    textColorReceived = true;
  }
  else if (strcmp(topic, "matrix/in/mode") == 0)
  {
    modeReceived = true;
  }

  // StaticJsonDocument<200> doc;
  // // Deserialize the JSON document
  // DeserializationError error = deserializeJson(doc, payload);

  // // Test if parsing succeeds.
  // if (error)
  // {
  //   Serial.print(F("deserializeJson() failed: "));
  //   Serial.println(error.c_str());
  //   return;
  // }
  // int instruction = doc["instruction"];

  // switch (instruction)
  // {
  // case BUTTON1:
  //   remoteButton1 = true;
  //   break;
  // case BUTTON2:
  //   remoteButton2 = true;
  //   break;
  // case MESSAGE:
  //   messageReceived = true;
  //   message = doc["message"].as<String>();
  //   break;
  // case PONG_PAUSE:
  //   pauseGame = true;
  // case PONG_RESUME:
  //   pauseGame = false;
  // default:
  //   break;
  // }
}

void Mqtt::init()
{
  mqttClient_.setServer(mqtt_host, mqtt_port);
  mqttClient_.connect((role_ + "/online").c_str(), mqtt_user, mqtt_password, "matrix/status/online", 1, true, "false");
  mqttClient_.subscribe("matrix/in/#");
  mqttClient_.setCallback([this](char *topic, byte *payload, unsigned int length) { this->callback(topic, payload, length); });

  sendOnlineMsg();
}

bool Mqtt::isButton1Pressed()
{
  bool result = remoteButton1;
  remoteButton1 = false;
  return result;
}

bool Mqtt::isButton2Pressed()
{
  bool result = remoteButton2;
  remoteButton2 = false;
  return result;
}

bool Mqtt::isMessageReceived()
{
  bool result = messageReceived;
  messageReceived = false;
  return result;
}

bool Mqtt::isBrightnessReceived()
{
  bool result = brightnessReceived;
  brightnessReceived = false;
  return result;
}

bool Mqtt::isFramerateReceived()
{
  bool result = framerateReceived;
  framerateReceived = false;
  return result;
}

bool Mqtt::isScrollDirectionReceived()
{
  bool result = scrollDirectionReceived;
  scrollDirectionReceived = false;
  return result;
}

bool Mqtt::isTextDirectionReceived()
{
  bool result = textDirectionReceived;
  textDirectionReceived = false;
  return result;
}

bool Mqtt::isTextColorReceived()
{
  bool result = textColorReceived;
  textColorReceived = false;
  return result;
}

bool Mqtt::isModeReceived()
{
  bool result = modeReceived;
  modeReceived = false;
  return result;
}

bool Mqtt::isGamePaused()
{
  return pauseGame;
}

bool Mqtt::isDiscoPaused()
{
  return pauseDisco;
}

bool Mqtt::isDiscoSpeedReceived()
{
  bool result = discoSpeedReceived;
  discoSpeedReceived = false;
  return result;
}

char *Mqtt::getMessage()
{
  return message;
}

void Mqtt::tick()
{
  static int lastTickTime = millis();
  mqttClient_.loop();

  if (millis() - lastTickTime > 5000)
  {
    if (!mqttClient_.connected())
    {
      reconnect();
    }
    lastTickTime = millis();
  }
}

void Mqtt::sendOnlineMsg()
{
  const char *pPayload = "true";
  int payloadLen = strlen(pPayload);

  mqttClient_.publish("matrix/status/online", reinterpret_cast<const uint8_t *>(pPayload), payloadLen, true);
}

void Mqtt::sendGameStartMsg()
{
  //  uint8_t* p = nullptr;
  //  mqttClient_.publish("matrix/espong/OnGameStart", p, 0);
}

void Mqtt::sendBallHitMsg(int pos, String direction, int speed)
{
  StaticJsonDocument<256> root;
  root["position"] = pos;
  root["direction"] = direction.c_str();
  root["speed"] = speed;

  char pMqtt[256];
  serializeJson(root, pMqtt);
  mqttClient_.publish("matrix/espong/OnBallHit", pMqtt, 256);
}

void Mqtt::sendPlayerVictoryMsg(int playerId)
{
  StaticJsonDocument<256> root;
  root["playerId"] = playerId;

  char pMqtt[256];
  serializeJson(root, pMqtt);
  mqttClient_.publish("matrix/espong/OnPlayerVictory", pMqtt, 256);
}
