/*
 * environment_sensors.h – Read onboard carrier sensors
 */

#ifndef ENVIRONMENT_SENSORS_H
#define ENVIRONMENT_SENSORS_H

#include "types.h"

class MKRIoTCarrier;

SensorReading readEnvironmentSensors(MKRIoTCarrier& carrier);

#endif /* ENVIRONMENT_SENSORS_H */
