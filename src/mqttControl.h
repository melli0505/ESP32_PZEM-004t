void initPWMptc(void);
void setup_mqtt(PubSubClient* client_instance);
void reconnect(void);
void updateMQTTwithPWM(int PwmValue, bool force);
void connectMQTTBroker(void);
void callback(char* topic, byte* payload, unsigned int length);
void publishAC(void);