#include "display_renderer.h"

#include "config.h"

#include <Arduino.h>
#include <Arduino_MKRIoTCarrier.h>
#include <stdio.h>

void setCursorCentered(MKRIoTCarrier& carrier, const char* text, int yPosition) {
  int16_t boundsX = 0;
  int16_t boundsY = 0;
  uint16_t textWidth = 0;
  uint16_t textHeight = 0;

  carrier.display.getTextBounds(text, 0, 0, &boundsX, &boundsY, &textWidth, &textHeight);
  carrier.display.setCursor((DISPLAY_WIDTH - (int)textWidth) / 2, yPosition);
}

void renderSensorScreen(
    MKRIoTCarrier& carrier,
    const SensorReading& reading,
    bool shakeDetected,
    const char* gestureLabel) {
  carrier.display.fillScreen(0x0000);
  carrier.display.setTextColor(0xFFFF);

  carrier.display.setTextSize(2);
  setCursorCentered(carrier, "Opla Demo", 16);
  carrier.display.print("Opla Demo");

  carrier.display.setTextSize(1);
  char lineBuffer[32];

  snprintf(lineBuffer, sizeof(lineBuffer), "Temp: %.1f C", (double)reading.temperatureCelsius);
  setCursorCentered(carrier, lineBuffer, 44);
  carrier.display.print(lineBuffer);

  snprintf(lineBuffer, sizeof(lineBuffer), "Humidity: %.0f %%", (double)reading.humidityPercent);
  setCursorCentered(carrier, lineBuffer, 56);
  carrier.display.print(lineBuffer);

  snprintf(lineBuffer, sizeof(lineBuffer), "Pressure: %.1f kPa", (double)reading.pressureKPa);
  setCursorCentered(carrier, lineBuffer, 68);
  carrier.display.print(lineBuffer);

  snprintf(lineBuffer, sizeof(lineBuffer), "Gesture: %s", gestureLabel);
  setCursorCentered(carrier, lineBuffer, 86);
  carrier.display.print(lineBuffer);

  if (shakeDetected) {
    carrier.display.setTextColor(0x07E0);
    setCursorCentered(carrier, "Shake!", 102);
    carrier.display.print("Shake!");
  } else {
    carrier.display.setTextColor(0xFFFF);
    setCursorCentered(carrier, "Btn 0-4: LED | 3: shake, 4: show", 102);
    carrier.display.print("Btn 0-4: LED | 3: shake, 4: show");
  }
}

void renderStartupScreen(MKRIoTCarrier& carrier) {
  carrier.display.fillScreen(0x0000);
  carrier.display.setTextColor(0x07E0);
  carrier.display.setTextSize(2);
  setCursorCentered(carrier, "Opla Demo", DISPLAY_HEIGHT / 2 - 24);
  carrier.display.print("Opla Demo");

  carrier.display.setTextSize(1);
  setCursorCentered(carrier, "Press any button to start", DISPLAY_HEIGHT / 2 + 4);
  carrier.display.print("Press any button to start");
}
