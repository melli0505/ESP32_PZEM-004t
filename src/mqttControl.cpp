#include <Arduino.h>
#include <sstream>
#include "config.h"

const char* mqttServer = "115.95.190.117"; //
const int mqttPort = 1883;

WiFiClient espClient; //
WiFiClient espClient2; //
PubSubClient mqttClient(espClient); //
PubSubClient subClient(espClient2);

const char* topic_pub = "Voltage"; //
const char* topic_sub = "Control"; //

int pwmValue = 0;

void initPWMptc(void);
void updateMQTTwithPWM(int PwmValue, bool force);
void tryReconnect(PubSubClient client, char* mode);
void subControlInst(char* topic, byte* payload, unsigned int length);

void connectMQTTBroker(void) {
  mqttClient.setServer(mqttServer, mqttPort);
  subClient.setServer(mqttServer, mqttPort);
  subClient.setCallback(subControlInst);
  if (!mqttClient.connected() || !subClient.connected()) {
    if (!mqttClient.connected()) {
      Serial.println("Connecting to MQTT as publisher..");
      tryReconnect(mqttClient, "Publisher");
    }
    if (!subClient.connected()) {
      Serial.println("Connecting to MQTT as subscriber..");
      tryReconnect(subClient, "Subscriber");
    }
  }
  
  subClient.subscribe(topic_sub);
  subClient.loop();
}

void tryReconnect(PubSubClient client, char* mode) {
  if (client.connect("Esp32Client", mode, "password")) {
    Serial.printf("Connected to MQTT as %s.\r\n", mode);
  }
  else {
    Serial.printf("Failed Connecting to MQTT. Retry in 5 seconds..\r\n");
    Serial.println(client.state());
    delay(5000);
  }
}

void initPWMptc(void){
  ledcSetup(pwmChannel, pwmFreq, pwmResolution);
  ledcAttachPin(pwmPin, pwmChannel);

  pwmValue = initialPwmValue;

  updateMQTTwithPWM(pwmValue, true);
  Serial.printf("  Fan PWM sucessfully initialized.\r\n");
}

void updateMQTTwithPWM(int PwmValue, bool force) {
  if ((pwmValue != PwmValue) || force) {
    pwmValue = PwmValue;
    if (pwmValue < 0) { pwmValue = 0; };
    if (pwmValue > 255) { pwmValue = 255; };
    ledcWrite(pwmChannel, pwmValue);

    // mqtt
    std::stringstream value;
    value<<pwmValue;

    mqttClient.publish(topic_pub, value.str().c_str());
  }
}

void subControlInst(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived in topic:");
  Serial.println(topic);
}