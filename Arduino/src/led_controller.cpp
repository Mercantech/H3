#include "led_controller.h"

#include "config.h"
#include "temperature_color.h"

#include <Arduino.h>
#include <Arduino_MKRIoTCarrier.h>

namespace {

const int kButtonColors[LED_COUNT][3] = {
    {255, 0, 0},
    {255, 128, 0},
    {255, 255, 0},
    {0, 255, 0},
    {0, 150, 255},
};

const touchButtons kTouchPads[LED_COUNT] = {
    TOUCH0, TOUCH1, TOUCH2, TOUCH3, TOUCH4,
};

}  // namespace

void renderRainbowLeds(MKRIoTCarrier& carrier, int colorOffset) {
  const int rainbowColors[LED_COUNT][3] = {
      {255, 0, 0}, {255, 128, 0}, {255, 255, 0}, {0, 255, 0}, {0, 100, 255},
  };

  for (int ledIndex = 0; ledIndex < LED_COUNT; ledIndex++) {
    const int paletteIndex = (ledIndex + colorOffset) % LED_COUNT;
    carrier.leds.setPixelColor(
        ledIndex,
        rainbowColors[paletteIndex][0],
        rainbowColors[paletteIndex][1],
        rainbowColors[paletteIndex][2]);
  }
  carrier.leds.show();
}

void renderTemperatureLeds(MKRIoTCarrier& carrier, float temperatureCelsius) {
  int red = 0;
  int green = 0;
  int blue = 0;
  temperatureToRgb(temperatureCelsius, red, green, blue);
  carrier.leds.fill(carrier.leds.Color(red, green, blue), 0, LED_COUNT);
  carrier.leds.show();
}

void turnOffLeds(MKRIoTCarrier& carrier) {
  carrier.leds.clear();
  carrier.leds.show();
}

void playLedShow(MKRIoTCarrier& carrier) {
  for (int flashIndex = 0; flashIndex < 3; flashIndex++) {
    carrier.leds.fill(carrier.leds.Color(255, 255, 255), 0, LED_COUNT);
    carrier.leds.show();
    delay(80);
    carrier.leds.clear();
    carrier.leds.show();
    delay(80);
  }
}

void renderButtonLeds(MKRIoTCarrier& carrier) {
  for (int buttonIndex = 0; buttonIndex < LED_COUNT; buttonIndex++) {
    if (carrier.Buttons.getTouch(kTouchPads[buttonIndex])) {
      carrier.leds.setPixelColor(
          buttonIndex,
          kButtonColors[buttonIndex][0],
          kButtonColors[buttonIndex][1],
          kButtonColors[buttonIndex][2]);
    } else {
      carrier.leds.setPixelColor(buttonIndex, 0, 0, 0);
    }
  }
  carrier.leds.show();
}
