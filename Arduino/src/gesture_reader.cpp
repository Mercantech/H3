#include "gesture_reader.h"

#include <Arduino.h>
#include <Arduino_MKRIoTCarrier.h>
#include <string.h>

void pollGestureLabel(MKRIoTCarrier& carrier, char* outLabel, size_t outLabelSize) {
  if (outLabelSize == 0) {
    return;
  }

  if (!carrier.Light.gestureAvailable()) {
    return;
  }

  const uint8_t gesture = carrier.Light.readGesture();
  const char* label = "-";

  if (gesture == UP) {
    label = "UP";
  } else if (gesture == DOWN) {
    label = "DOWN";
  } else if (gesture == LEFT) {
    label = "LEFT";
  } else if (gesture == RIGHT) {
    label = "RIGHT";
  }

  strncpy(outLabel, label, outLabelSize - 1);
  outLabel[outLabelSize - 1] = '\0';
}
