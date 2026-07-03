/*
 * led_controller.h – RGB ring on the Oplà carrier
 */

#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

class MKRIoTCarrier;

void renderRainbowLeds(MKRIoTCarrier& carrier, int colorOffset);
void renderTemperatureLeds(MKRIoTCarrier& carrier, float temperatureCelsius);
void turnOffLeds(MKRIoTCarrier& carrier);
void playLedShow(MKRIoTCarrier& carrier);
void renderButtonLeds(MKRIoTCarrier& carrier);

#endif /* LED_CONTROLLER_H */
