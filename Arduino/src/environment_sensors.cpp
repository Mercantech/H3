#include "environment_sensors.h"

#include <Arduino_MKRIoTCarrier.h>

SensorReading readEnvironmentSensors(MKRIoTCarrier& carrier) {
  SensorReading reading;
  reading.temperatureCelsius = carrier.Env.readTemperature();
  reading.humidityPercent = carrier.Env.readHumidity();
  reading.pressureKPa = carrier.Pressure.readPressure();
  return reading;
}
