/*
 * shake_detector.h – IMU shake detection
 */

#ifndef SHAKE_DETECTOR_H
#define SHAKE_DETECTOR_H

class MKRIoTCarrier;

bool detectShake(
    MKRIoTCarrier& carrier,
    bool shakeModeEnabled,
    unsigned long& lastShakeTimestampMs);

#endif /* SHAKE_DETECTOR_H */
