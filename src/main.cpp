#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <sstream>

#include "config.h"
#include "wifiControl.h"
#include "mqttControl.h"

int value = initialPwmValue;

void setup() {
  Serial.begin(9600);
  Serial.println("");

  // connect with WiFi
  WiFiInit();
  WiFiEnable();

  // connect with MQTT
  connectMQTTBroker();

  // pwm initialize
  initPWMptc();
  Serial.printf("Setting done, have fun.\r\n");
}

void loop() {
  value = random(20, 26);
  updateMQTTwithPWM(value, true);
  delay(10000);
  Serial.println("test complete.");
}



