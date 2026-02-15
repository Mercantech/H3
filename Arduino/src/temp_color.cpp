/*
 * temp_color.cpp – Temperatur → RGB (ren logik, ingen Arduino)
 *
 * Kan kompileres til både firmware og native test.
 */

#include "temp_color.h"

void tempToRgb(float temp, int& r, int& g, int& b) {
  if (temp < 15) {
    r = 0;
    g = 50;
    b = 255;
  } else if (temp < 22) {
    int t = (int)((temp - 15) * 36);
    r = t;
    g = 100;
    b = 255 - t;
  } else {
    r = 255;
    g = (int)(255 - (temp - 22) * 20);
    if (g < 0) g = 0;
    b = 0;
  }
}
