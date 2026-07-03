#include "shake_detector.h"

#include "config.h"

#include <Arduino.h>
#include <Arduino_MKRIoTCarrier.h>
#include <math.h>

bool detectShake(
    MKRIoTCarrier& carrier,
    bool shakeModeEnabled,
    unsigned long& lastShakeTimestampMs) {
  if (!shakeModeEnabled || !carrier.IMUmodule.accelerationAvailable()) {
    return false;
  }

  float accelerationX = 0.0f;
  float accelerationY = 0.0f;
  float accelerationZ = 0.0f;
  carrier.IMUmodule.readAcceleration(accelerationX, accelerationY, accelerationZ);

  const float magnitude = sqrtf(
      accelerationX * accelerationX +
      accelerationY * accelerationY +
      accelerationZ * accelerationZ);

  const unsigned long nowMs = millis();
  if (magnitude > SHAKE_THRESHOLD && (nowMs - lastShakeTimestampMs) > SHAKE_DEBOUNCE_MS) {
    lastShakeTimestampMs = nowMs;
    return true;
  }

  return false;
}
