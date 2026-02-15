/*
 * display_utils.cpp – Implementering af display-hjælpefunktioner
 *
 * Alt der har med den runde skærm at gøre: centrering og opdatering.
 */

#include "display_utils.h"
#include "config.h"
#include <Arduino_MKRIoTCarrier.h>
#include <Arduino.h>
#include <stdio.h>

void setCursorCentered(MKRIoTCarrier& carrier, const char* str, int y) {
  int16_t x1, y1;
  uint16_t w, h;
  carrier.display.getTextBounds(str, 0, 0, &x1, &y1, &w, &h);
  carrier.display.setCursor((DISPLAY_W - (int)w) / 2, y);
}

void updateDisplay(MKRIoTCarrier& carrier, float temp, float humidity, float pressure,
                  bool showShake, const char* lastGestureStr) {
  carrier.display.fillScreen(0x0000);
  carrier.display.setTextColor(0xFFFF);

  carrier.display.setTextSize(2);
  setCursorCentered(carrier, "Opla Demo", 16);
  carrier.display.print("Opla Demo");

  carrier.display.setTextSize(1);
  char buf[32];

  snprintf(buf, sizeof(buf), "Temp: %.1f C", (double)temp);
  setCursorCentered(carrier, buf, 44);
  carrier.display.print(buf);

  snprintf(buf, sizeof(buf), "Fugt: %.0f %%", (double)humidity);
  setCursorCentered(carrier, buf, 56);
  carrier.display.print(buf);

  snprintf(buf, sizeof(buf), "Tryk: %.1f kPa", (double)pressure);
  setCursorCentered(carrier, buf, 68);
  carrier.display.print(buf);

  snprintf(buf, sizeof(buf), "Gesture: %s", lastGestureStr);
  setCursorCentered(carrier, buf, 86);
  carrier.display.print(buf);

  if (showShake) {
    carrier.display.setTextColor(0x07E0);
    setCursorCentered(carrier, "Ryst!", 102);
    carrier.display.print("Ryst!");
  } else {
    carrier.display.setTextColor(0xFFFF);
    setCursorCentered(carrier, "Knap 0-4: lys | 3: ryst, 4: show", 102);
    carrier.display.print("Knap 0-4: lys | 3: ryst, 4: show");
  }
}

void showStartScreen(MKRIoTCarrier& carrier) {
  carrier.display.fillScreen(0x0000);
  carrier.display.setTextColor(0x07E0);
  carrier.display.setTextSize(2);
  setCursorCentered(carrier, "Opla Demo", DISPLAY_H / 2 - 24);
  carrier.display.print("Opla Demo");
  carrier.display.setTextSize(1);
  setCursorCentered(carrier, "Tryk en knap for at starte", DISPLAY_H / 2 + 4);
  carrier.display.print("Tryk en knap for at starte");
}
