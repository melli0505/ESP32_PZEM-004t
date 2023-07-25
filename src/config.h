#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

static const String wifi_ssid = "tplink-eap 2.4GHz";
static const String wifi_password = "tplink-eap";

const int pwmChannel = 0;
const int pwmFreq = 5000;
const int pwmResolution = 8;

const int pwmPin = 0;
const int initialPwmValue = 120;

static const char* mqttServer = "115.95.190.117"; 
static const int mqttPort = 1883;

static const char* mqtt_user              = "myuser";
static const char* mqtt_pass              = "myPassword";
static const char* mqtt_clientName        = "esp32_arduino";

static unsigned long reconnectInterval = 5000;
static unsigned long lastReconnectAttempt = millis() - reconnectInterval - 1;

static const char* topic_sub = "Control"; 
