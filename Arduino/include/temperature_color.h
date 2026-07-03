/*
 * temperature_color.h – Map temperature to RGB (pure logic, no hardware)
 */

#ifndef TEMPERATURE_COLOR_H
#define TEMPERATURE_COLOR_H

/** Compute RGB (0–255) from temperature °C: cold = blue, warm = red. */
void temperatureToRgb(float temperatureCelsius, int& red, int& green, int& blue);

#endif /* TEMPERATURE_COLOR_H */
