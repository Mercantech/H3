/*
 * temp_color.h – Temperatur → RGB (ren logik, ingen hardware)
 *
 * Bruges af led_utils og kan testes uden Arduino.
 */

#ifndef TEMP_COLOR_H
#define TEMP_COLOR_H

/** Beregn RGB (0–255) ud fra temperatur °C: kold = blå, varm = rød. */
void tempToRgb(float temp, int& r, int& g, int& b);

#endif /* TEMP_COLOR_H */
