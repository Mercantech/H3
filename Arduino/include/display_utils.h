/*
 * display_utils.h – Display-hjælpefunktioner til den runde Oplà-skærm
 *
 * Centrering af tekst og opdatering af demo-skærmen.
 * Implementering i display_utils.cpp
 */

#ifndef DISPLAY_UTILS_H
#define DISPLAY_UTILS_H

class MKRIoTCarrier;

/** Sæt cursor så teksten bliver centreret horisontalt på skærmen (y er fast). */
void setCursorCentered(MKRIoTCarrier& carrier, const char* str, int y);

/** Opdater hovedskærmen med sensorværdier og status. */
void updateDisplay(MKRIoTCarrier& carrier, float temp, float humidity, float pressure,
                  bool showShake, const char* lastGestureStr);

/** Vis startskærmen ("Opla Demo" + "Tryk en knap for at starte"). */
void showStartScreen(MKRIoTCarrier& carrier);

#endif /* DISPLAY_UTILS_H */
