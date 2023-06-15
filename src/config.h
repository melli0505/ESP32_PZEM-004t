#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

const String wifi_ssid = "tplink-eap 2.4GHz";
const String wifi_password = "tplink-eap";

const int pwmChannel = 0;
const int pwmFreq = 5000;
const int pwmResolution = 8;

const int pwmPin = 0;
const int initialPwmValue = 120;
