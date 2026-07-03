/*
 * display_renderer.h – Round Oplà display helpers
 */

#ifndef DISPLAY_RENDERER_H
#define DISPLAY_RENDERER_H

#include "types.h"

class MKRIoTCarrier;

void setCursorCentered(MKRIoTCarrier& carrier, const char* text, int yPosition);

void renderSensorScreen(
    MKRIoTCarrier& carrier,
    const SensorReading& reading,
    bool shakeDetected,
    const char* gestureLabel);

void renderStartupScreen(MKRIoTCarrier& carrier);

#endif /* DISPLAY_RENDERER_H */
