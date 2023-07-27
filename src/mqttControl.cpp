#include <Arduino.h>
#include <sstream>
#include <WiFi.h>
#include <PubSubClient.h>

#include "config.h"
#include "pzemControl.h"

void connectMQTTBroker(void);
void reconnect(void);
bool publishMQTTMessage(const char *topic, const char *payload);
void callback(char* topic, byte* payload, unsigned int length);
void initPWMptc(void);
void updateMQTTwithPWM(int PwmValue, bool force);

static PubSubClient* client;

int pwmValue = 0;

void setup_mqtt(PubSubClient* client_instance) {
  client = client_instance;
  if (!client->connected()) {
    unsigned long currentMillis = millis();
    if ((currentMillis - lastReconnectAttempt) > reconnectInterval){
      lastReconnectAttempt = currentMillis;
      Serial.println("Connecting to MQTT as publisher..");
      reconnect();
    }
  }

  Serial.printf("\tmqttClient loop started.\n");
  Serial.println("Setting up MQTT Client");
  
  client->subscribe(topic_sub);
}


void reconnect() {
  while (!client->connected())
  {
    Serial.print("Attempting MQTT Connection...");
    if (client->connect((char*) mqtt_clientName, (char*) mqtt_user, (char*) mqtt_pass)) {
      Serial.println("Connected.");
      client->subscribe(topic_sub);
      client->loop();
    } else {
      Serial.print("Failed. rc = ");
      Serial.println(client->state());
      Serial.println("Try again in 5 seconds.");
      delay(5000);
    }
  }
}



bool publishMQTTMessage(const char *topic, const char *payload) {
  /*
  Publish message on MQTT Server.
  */

  if (client->publish(topic, payload)){
    return true;
  } else {
    Serial.println("Cannot publish mqtt message.");
    return false;
  }
}


void callback(char* topic, byte* message, unsigned int length) {
  /*
  Callback function that calls when message from subscribed topic has been arrived.
  */
  
  String payload;
  for (int i = 0; i < length; i++) {
    payload += (char)message[i];
  }

  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  Serial.print("Message arrived: ");
  Serial.println(payload);

  if (payload == "reset") {
    bool isSucceed = reset_energy_counter();
    Serial.print("Reset Energy Counter : ");
    Serial.println(isSucceed);
  }
}


void initPWMptc(){
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

    std::stringstream value;
    value<<pwmValue;

    publishMQTTMessage("Temp_Topic", value.str().c_str());
  }
}


void publishAC() {
  float voltage = get_voltage();
  float power = get_power();
  float energy = get_energy();
  float current = get_current();
  float frequency = get_frequency();
  float pf = get_pf();

  Serial.println(voltage); 
  Serial.println(power); 
  Serial.println(energy);
  Serial.println(current); 
  Serial.println(frequency); 
  Serial.println(pf);


  std::string data = "{ \"voltage\" : " + std::to_string(voltage) 
  + ", \"power\" : " + std::to_string(power)
  + ", \"energe\" : " + std::to_string(energy) 
  + ", \"current\" : " + std::to_string(current) 
  + ", \"frequency\" : " + std::to_string(frequency) 
  + ", \"pf\" : " + std::to_string(pf) + "}"; 

  publishMQTTMessage("data", data.c_str());
}