/*
 * led_utils.cpp – Implementering af LED-hjælpefunktioner
 *
 * Alt der styrer de 5 RGB-LEDs på carrieren.
 */

#include "led_utils.h"
#include "config.h"
#include "temp_color.h"
#include <Arduino_MKRIoTCarrier.h>
#include <Arduino.h>

void ledsRainbow(MKRIoTCarrier& carrier, int offset) {
  const int colors[5][3] = {
    {255, 0, 0}, {255, 128, 0}, {255, 255, 0}, {0, 255, 0}, {0, 100, 255}
  };
  for (int i = 0; i < LED_COUNT; i++) {
    int idx = (i + offset) % LED_COUNT;
    carrier.leds.setPixelColor(i, colors[idx][0], colors[idx][1], colors[idx][2]);
  }
  carrier.leds.show();
}

void ledsFromTemperature(MKRIoTCarrier& carrier, float temp) {
  int r, g, b;
  tempToRgb(temp, r, g, b);
  carrier.leds.fill(carrier.leds.Color(r, g, b), 0, LED_COUNT);
  carrier.leds.show();
}

void ledsOff(MKRIoTCarrier& carrier) {
  carrier.leds.clear();
  carrier.leds.show();
}

void ledsQuickShow(MKRIoTCarrier& carrier) {
  for (int i = 0; i < 3; i++) {
    carrier.leds.fill(carrier.leds.Color(255, 255, 255), 0, LED_COUNT);
    carrier.leds.show();
    delay(80);
    carrier.leds.clear();
    carrier.leds.show();
    delay(80);
  }
}

// Hver knap har sin egen farve (R, G, B)
static const int BUTTON_COLORS[5][3] = {
  {255, 0, 0},   /* 0: rød */
  {255, 128, 0}, /* 1: orange */
  {255, 255, 0}, /* 2: gul */
  {0, 255, 0},   /* 3: grøn */
  {0, 150, 255}  /* 4: blå */
};

void ledsFromButtons(MKRIoTCarrier& carrier) {
  const touchButtons touchPads[5] = { TOUCH0, TOUCH1, TOUCH2, TOUCH3, TOUCH4 };
  for (int i = 0; i < LED_COUNT; i++) {
    if (carrier.Buttons.getTouch(touchPads[i])) {
      carrier.leds.setPixelColor(i, BUTTON_COLORS[i][0], BUTTON_COLORS[i][1], BUTTON_COLORS[i][2]);
    } else {
      carrier.leds.setPixelColor(i, 0, 0, 0);
    }
  }
  carrier.leds.show();
}
