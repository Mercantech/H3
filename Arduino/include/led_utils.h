/*
 * led_utils.h – LED-hjælpefunktioner (RGB-ringe på Oplà)
 *
 * Temaer: regnbue, temperatur-farve, slukket, og et kort “show”.
 * Implementering i led_utils.cpp
 */

#ifndef LED_UTILS_H
#define LED_UTILS_H

class MKRIoTCarrier;

/** Regnbue-tema med roterende farver (offset styrer start). */
void ledsRainbow(MKRIoTCarrier& carrier, int offset);

/** Farve efter temperatur: kold = blå, varm = rød. */
void ledsFromTemperature(MKRIoTCarrier& carrier, float temp);

/** Sluk alle LEDs. */
void ledsOff(MKRIoTCarrier& carrier);

/** Kort hvidt blink-show (3 gange). */
void ledsQuickShow(MKRIoTCarrier& carrier);

/** Sæt hver LED ud fra den tilhørende knap: tryk = tændt i egen farve, slip = slukket. */
void ledsFromButtons(MKRIoTCarrier& carrier);

#endif /* LED_UTILS_H */
