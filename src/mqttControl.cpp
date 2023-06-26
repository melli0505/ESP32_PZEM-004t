#include <Arduino.h>
#include <sstream>
#include <WiFi.h>
#include "config.h"

const char* mqttServer = "115.95.190.117"; //
const int mqttPort = 1883;

const char* const mqtt_user              = "myuser";
const char* const mqtt_pass              = "myPassword";
const char* const mqtt_clientName        = "esp32_arduino";

unsigned long reconnectInterval = 5000;
unsigned long lastReconnectAttempt = millis() - reconnectInterval - 1;

const char* topic_pub = "Voltage"; //
const char* topic_sub = "Control"; //

int pwmValue = 0;

void connectMQTTBroker(void);
bool reconnect(void);
bool publishMQTTMessage(const char *topic, const char *payload);
void callback(char* topic, byte* payload, unsigned int length);

void initPWMptc(void);
void updateMQTTwithPWM(int PwmValue, bool force);
WiFiClient espClient; //
PubSubClient mqttClient(mqttServer, mqttPort, callback, espClient); //


void connectMQTTBroker(void) {
  /*
  Connect MQTT Server and start MQTT Client loop.
  */

  if (!mqttClient.connected()) {
    unsigned long currentMillis = millis();
    if ((currentMillis - lastReconnectAttempt) > reconnectInterval){
      lastReconnectAttempt = currentMillis;
      Serial.println("Connecting to MQTT as publisher..");
      reconnect();
    }
  }

  if (mqttClient.connected()) { mqttClient.loop(); }
  Serial.printf("\tmqttClient loop started.\n");
}


bool reconnect(void) {
  /*
  Try to reconnect MQTT Server.
  */

  if (WiFi.isConnected()) {
    if (mqttClient.connected()) { return true; }
    else {
      if (mqttClient.connect((char*) mqtt_clientName, (char*) mqtt_user, (char*) mqtt_pass)) {
        Serial.printf("Connected to MQTT broker.");
        mqttClient.subscribe("Control");
      } 
      else {
        Serial.printf("Failed Connecting to MQTT. Retry in 5 seconds..\r\n");
        Serial.println(mqttClient.state());
        delay(5000);
      }
      return mqttClient.connected();
    }
  } else {
    printf("No connection to MQTT server, because WiFi not available.");
    return false;
  }
}


bool publishMQTTMessage(const char *topic, const char *payload) {
  /*
  Publish message on MQTT Server.
  */

  if (!WiFi.isConnected()) return false;

  if (reconnect()) {
    if (mqttClient.publish(topic, payload)) {
      return true;
    }
    else {
      Serial.printf("\t Cannot publish mqtt message.");
    }
  }
  else {
    printf("\t Cannot publish message because MQTT Server connection failed.");
  }
  return false;
}


void callback(char* topic, byte* payload, unsigned int length) {
  /*
  Callback function that calls when message from subscribed topic has been arrived.
  */

  Serial.printf("Message arrived!!!!!!!!!!!!!!!!!!\n");
  std::string strPayload(reinterpret_cast<const char *>(payload), length);

  Serial.printf("Message arrived in topic: %s", topic);

  Serial.printf("Message arrived: %s", strPayload.c_str());
}


void initPWMptc(void){
  /*
  PWM control initialize.
  */

  ledcSetup(pwmChannel, pwmFreq, pwmResolution);
  ledcAttachPin(pwmPin, pwmChannel);

  pwmValue = initialPwmValue;

  updateMQTTwithPWM(pwmValue, true);
  Serial.printf("  Fan PWM sucessfully initialized.\r\n");
}


void updateMQTTwithPWM(int PwmValue, bool force) {
  /*
  Update PWM value and publish on MQTT server.
  */
 
  if ((pwmValue != PwmValue) || force) {
    pwmValue = PwmValue;
    if (pwmValue < 0) { pwmValue = 0; };
    if (pwmValue > 255) { pwmValue = 255; };
    ledcWrite(pwmChannel, pwmValue);

    // mqtt
    std::stringstream value;
    value<<pwmValue;

    publishMQTTMessage("Voltage", value.str().c_str());
  }
}

