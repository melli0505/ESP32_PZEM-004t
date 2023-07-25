#include <WiFi.h>
#include <Wire.h>
#include "config.h"

boolean connected = false;
String accessPointName;

void WiFiConnected(WiFiEvent_t event, WiFiEventInfo_t info);
void WiFiDisconnected(WiFiEvent_t event, WiFiEventInfo_t info);
void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info);
void printWiFiStatus();

void setup_wifi() {
  delay(10);
  Serial.print("connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin((const char*)wifi_ssid.c_str(), (const char*)wifi_password.c_str());
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }
  Serial.println("wifi connected.");
}

// void WiFiInit(void) {
//   WiFi.onEvent(WiFiConnected, ARDUINO_EVENT_WIFI_AP_STACONNECTED);
//   WiFi.onEvent(WiFiGotIP, ARDUINO_EVENT_WIFI_STA_GOT_IP);
//   WiFi.onEvent(WiFiDisconnected, ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

//   WiFi.mode(WIFI_MODE_STA);
// }

// void WiFiEnable(void) {
//   WiFi.onEvent(WiFiDisconnected, ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
//   WiFi.begin(wifi_ssid, wifi_password);
//   while(!connected) {
//     delay(3000);
//     Serial.println("Connecting to WiFi..");
//   }
//   printWiFiStatus();
// }

// void WiFiConnected(WiFiEvent_t event, WiFiEventInfo_t info) {
//   Serial.printf("Callback, WiFi Connected.\r\n");
//   connected = true;
// }

// void WiFiDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
//   Serial.printf("  Callback \"StationDisconnected\"\r\n");
//   connected = false;
//   printWiFiStatus();
// }

// void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info) {
//   Serial.printf("Callback, Got IP.\r\n");
//   connected = true;

//   String BSSID = String(WiFi.BSSIDstr());
//   if (BSSID == "00:11:22:33:44:55") {
//       accessPointName = "Your AP 2,4 GHz";
//   } else if (BSSID == "66:77:88:99:AA:BB") {
//       accessPointName = "Your AP 5 GHz";
//   }

//   printWiFiStatus();

// }

// void printWiFiStatus(void){

//   if (WiFi.isConnected()) {
//     Serial.printf("  WiFi.status() == connected\r\n");
//   } else {
//     Serial.printf("  WiFi.status() == DIS-connected\r\n");
//   }

//   Serial.printf(("  IP address: %s\r\n"), WiFi.localIP().toString().c_str());

//   if (WiFi.isConnected()) { //  && WiFi.localIP().isSet()) {
//     Serial.printf("  WiFi connected and IP is set :-)\r\n");
//   } else {
//     Serial.printf("  WiFi not completely available :-(\r\n");
//   }
// }
