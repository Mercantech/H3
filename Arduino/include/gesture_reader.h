/*
 * gesture_reader.h – Proximity gesture labels for the display
 */

#ifndef GESTURE_READER_H
#define GESTURE_READER_H

#include <stddef.h>

class MKRIoTCarrier;

/** Poll gesture sensor and write a short label into outLabel (e.g. "UP", "-"). */
void pollGestureLabel(MKRIoTCarrier& carrier, char* outLabel, size_t outLabelSize);

#endif /* GESTURE_READER_H */
