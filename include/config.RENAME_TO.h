#ifndef __CONFIG_H
#define __CONFIG_H

// ---------------------------------------------------------------
// !WARNING! >> DO NOT CHECK IN LOG IN DATA INTO GIT! << !WARNING!
// ---------------------------------------------------------------

// WIFI:
static const char *wifi_ssid = "wifiSSID";
static const char *wifi_password = "wifipw";
static const char *wifi_ota_name = "matrix";
static const char *wifi_ota_password = "matrix";

// mqtt:
static const char *mqtt_host = "mqttbroker";
static const char *mqtt_user = "user";
static const char *mqtt_password = "pw";
static const int mqtt_port = 1883;

#define LED_DATA_PIN 14 // Data pin where LED stripe is connected on Node MCU

#define MATRIX_WIDTH 32 * 3
#define MATRIX_HEIGHT -8
#define MATRIX_TYPE VERTICAL_ZIGZAG_MATRIX

// LED Stripe:

// Common settings:
// ================
// Bar
// -----------------------
#define NUM_LEDS 768      // Number of LEDs on the stripe
#define BUTTON_1_PIN 4    // Button of player 1
#define BUTTON_2_PIN 0    // Button of Player 2
#define RANGE 8           // Number of base LEDs per player
#define LED_BRIGHTNESS 10 // Brightness of LEDs (255 is 100%)
#define INITIAL_SPEED 50  // Initial speed
// -------------------------

#endif // !__CONFIG_H
