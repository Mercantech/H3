#include "wifi_manager.h"

#include "config.h"
#include "secrets.h"

#if USE_WIFI
#include <WiFiNINA.h>
#endif

#include <Arduino.h>

void connectWifi() {
#if USE_WIFI
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("WiFi connecting");

  for (int attempt = 0; attempt < WIFI_CONNECT_ATTEMPTS && WiFi.status() != WL_CONNECTED; attempt++) {
    delay(WIFI_CONNECT_RETRY_MS);
    Serial.print(".");
  }

  Serial.println();
  if (isWifiConnected()) {
    Serial.print("WiFi OK: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("WiFi failed – continuing without network");
  }
#else
  Serial.println("WiFi disabled in config.h");
#endif
}

bool isWifiConnected() {
#if USE_WIFI
  return WiFi.status() == WL_CONNECTED;
#else
  return false;
#endif
}
