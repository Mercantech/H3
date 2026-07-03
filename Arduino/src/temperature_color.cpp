#include "temperature_color.h"

void temperatureToRgb(float temperatureCelsius, int& red, int& green, int& blue) {
  if (temperatureCelsius < 15.0f) {
    red = 0;
    green = 50;
    blue = 255;
  } else if (temperatureCelsius < 22.0f) {
    int blend = (int)((temperatureCelsius - 15.0f) * 36.0f);
    red = blend;
    green = 100;
    blue = 255 - blend;
  } else {
    red = 255;
    green = (int)(255.0f - (temperatureCelsius - 22.0f) * 20.0f);
    if (green < 0) {
      green = 0;
    }
    blue = 0;
  }
}
