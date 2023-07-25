#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <sstream>
#include <PZEM004Tv30.h>

#include "config.h"
#include "wifiControl.h"
#include "mqttControl.h"
#include "pzemControl.h"

#define PZEM_RX_PIN 16
#define PZEM_TX_PIN 17

#define PZEM_SERIAL Serial2
#define CONSOLE_SERIAL Serial

int value = initialPwmValue;
unsigned long previousMillis10000Cycle = 0;
unsigned long interval10000Cycle = 5000;

IPAddress local_ip;
WiFiClient espClient;

static PubSubClient client(mqttServer, mqttPort, callback, espClient);
static PZEM004Tv30 pzem(&PZEM_SERIAL, PZEM_RX_PIN, PZEM_TX_PIN);

void setup() {
  Serial.begin(115200);
  Serial.println("");

  // wifi initialize
  setup_wifi();

  // mqtt initialize
  client.setClient(espClient);
  client.setServer(mqttServer, mqttPort);
  setup_mqtt(&client);

  // pzem-004t initialize
  setup_pzem(&pzem);

  // pwm initialize
  initPWMptc();
  
  Serial.printf("Setting done, have fun.\r\n");
}

void loop() {
  Serial.println("Loop working...");
  if(WiFi.status() != WL_CONNECTED) {
    setup_wifi();
  }

  if(!client.connected()) {
    reconnect();
  }

  client.loop();

  unsigned long currentMillis = millis();

  if ((currentMillis - previousMillis10000Cycle) >= interval10000Cycle) {
    previousMillis10000Cycle = currentMillis;

    // value = random(20, 26);
    // updateMQTTwithPWM(value, true);
    publishAC();

  }
  
  delay(7000);
}