#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <sstream>

#include "config.h"
#include "wifiControl.h"
#include "mqttControl.h"

int value = initialPwmValue;
unsigned long previousMillis10000Cycle = 0;
unsigned long interval10000Cycle = 50000;

void setup() {
  Serial.begin(9600);
  Serial.println("");

  // connect with WiFi
  WiFiInit();
  WiFiEnable();

  // pwm initialize
  initPWMptc();
  Serial.printf("Setting done, have fun.\r\n");
}

void loop() {
  connectMQTTBroker();

  unsigned long currentMillis = millis();

  // 50000ms 마다 한 번 씩 publishing call
  if ((currentMillis - previousMillis10000Cycle) >= interval10000Cycle) {
    previousMillis10000Cycle = currentMillis;

    value = random(20, 26);
    updateMQTTwithPWM(value, true);

  }
  
  delay(5000);
  // Serial.println("test complete.");
}