/*
 * Oplà IoT Carrier – Sjov demo
 *
 * Opdeling af kode (til undervisning):
 *   - config.h         = indstillinger (versioneres)
 *   - secrets.h        = WiFi/passwords (i .gitignore)
 *   - display_utils.*  = alt der vises på den runde skærm
 *   - led_utils.*      = styring af de 5 RGB-LEDs
 *   - main.cpp         = kun setup, loop og logik (touch, sensor, gesture)
 *
 * De 5 knapper styrer hver sin LED (knap 0 → LED 0 osv.). Touch 3: ryst on/off, Touch 4: kort LED-show.
 * Ingen lyd – egnet til mange enheder i samme lokale.
 */

#include <Arduino.h>
#include <math.h>
#include <Arduino_MKRIoTCarrier.h>

#include "config.h"
#include "secrets.h"
#include "display_utils.h"
#include "led_utils.h"

#if USE_WIFI
#include <WiFiNINA.h>
#endif

MKRIoTCarrier carrier;

// Tilstand
unsigned long lastShakeTime = 0;
const char* lastGestureStr = "-";
unsigned long lastDisplayUpdate = 0;

void setup() {
  Serial.begin(SERIAL_BAUD);

#if CARRIER_USE_CASE
  carrier.withCase();
#else
  carrier.noCase();
#endif
  carrier.begin();
  carrier.leds.setBrightness(LED_BRIGHTNESS);

  showStartScreen(carrier);

#if USE_WIFI
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("WiFi forbinder...");
  for (int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("OK: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("WiFi fejlede – demo kører uden net");
  }
#endif

  delay(1500);
}

void loop() {
  carrier.Buttons.update();

  float temp = carrier.Env.readTemperature();
  float humidity = carrier.Env.readHumidity();
  float pressure = carrier.Pressure.readPressure();

  static bool shakeMode = false;
  if (carrier.Buttons.onTouchDown(TOUCH3)) shakeMode = !shakeMode;
  if (carrier.Buttons.onTouchDown(TOUCH4)) ledsQuickShow(carrier);

  if (carrier.Light.gestureAvailable()) {
    uint8_t g = carrier.Light.readGesture();
    if (g == UP)       lastGestureStr = "OP";
    else if (g == DOWN)  lastGestureStr = "NED";
    else if (g == LEFT)  lastGestureStr = "VEN";
    else if (g == RIGHT) lastGestureStr = "HOEJ";
  }

  bool didShake = false;
  if (shakeMode && carrier.IMUmodule.accelerationAvailable()) {
    float x, y, z;
    carrier.IMUmodule.readAcceleration(x, y, z);
    float mag = sqrt(x*x + y*y + z*z);
    if (mag > SHAKE_THRESHOLD && (millis() - lastShakeTime) > SHAKE_DEBOUNCE_MS) {
      lastShakeTime = millis();
      didShake = true;
    }
  }

  // Hver knap tænder sin egen LED (knap 0 → LED 0 osv.)
  ledsFromButtons(carrier);

  if (millis() - lastDisplayUpdate >= DISPLAY_INTERVAL_MS) {
    lastDisplayUpdate = millis();
    updateDisplay(carrier, temp, humidity, pressure, didShake, lastGestureStr);
  }

  delay(30);
}
