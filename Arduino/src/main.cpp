/*
 * Oplà IoT Carrier – teaching demo
 *
 * Structure:
 *   config.h / secrets.h     – settings and credentials
 *   types.h                  – shared structs
 *   environment_sensors.*    – read temperature, humidity, pressure
 *   display_renderer.*       – round display output
 *   led_controller.*         – RGB ring
 *   shake_detector.*         – IMU shake detection
 *   gesture_reader.*         – proximity gestures
 *   wifi_manager.*           – optional WiFi connect
 *   temperature_color.*      – pure logic (unit-testable on PC)
 *   main.cpp                 – setup(), loop(), wiring only
 */

#include <Arduino.h>
#include <Arduino_MKRIoTCarrier.h>

#include "config.h"
#include "display_renderer.h"
#include "environment_sensors.h"
#include "gesture_reader.h"
#include "led_controller.h"
#include "shake_detector.h"
#include "wifi_manager.h"

MKRIoTCarrier carrier;

namespace {

bool shakeModeEnabled = false;
char lastGestureLabel[8] = "-";
unsigned long lastShakeTimestampMs = 0;
unsigned long lastDisplayUpdateMs = 0;

void initializeCarrier() {
#if CARRIER_USE_CASE
  carrier.withCase();
#else
  carrier.noCase();
#endif

  carrier.begin();
  carrier.leds.setBrightness(LED_BRIGHTNESS);
}

}  // namespace

void setup() {
  Serial.begin(SERIAL_BAUD);
  initializeCarrier();
  renderStartupScreen(carrier);
  connectWifi();
  delay(STARTUP_SPLASH_DELAY_MS);
}

void loop() {
  carrier.Buttons.update();

  const SensorReading reading = readEnvironmentSensors(carrier);

  if (carrier.Buttons.onTouchDown(TOUCH_INDEX_SHAKE_MODE)) {
    shakeModeEnabled = !shakeModeEnabled;
  }

  if (carrier.Buttons.onTouchDown(TOUCH_INDEX_LED_SHOW)) {
    playLedShow(carrier);
  }

  pollGestureLabel(carrier, lastGestureLabel, sizeof(lastGestureLabel));

  const bool shakeDetected = detectShake(carrier, shakeModeEnabled, lastShakeTimestampMs);

  renderButtonLeds(carrier);

  const unsigned long nowMs = millis();
  if (nowMs - lastDisplayUpdateMs >= DISPLAY_INTERVAL_MS) {
    lastDisplayUpdateMs = nowMs;
    renderSensorScreen(carrier, reading, shakeDetected, lastGestureLabel);
  }

  delay(LOOP_DELAY_MS);
}
