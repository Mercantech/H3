/*
 * types.h – Shared data types (no hardware dependencies)
 */

#ifndef TYPES_H
#define TYPES_H

struct SensorReading {
  float temperatureCelsius;
  float humidityPercent;
  float pressureKPa;
};

#endif /* TYPES_H */
