#include <WiFi.h>

void setup_wifi(void);
void WiFiInit(void);
void WiFiEnable(void);

void WiFiConnected(WiFiEvent_t event, WiFiEventInfo_t info);
void WiFiDisconnected(WiFiEvent_t event, WiFiEventInfo_t info);
void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info);
void printWiFiStatus();