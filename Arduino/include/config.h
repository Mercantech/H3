/*
 * config.h – Project configuration (no secrets)
 *
 * Safe to commit. WiFi passwords and API keys belong in secrets.h.
 */

#ifndef CONFIG_H
#define CONFIG_H

// ----- Serial -----
#define SERIAL_BAUD 9600

// ----- Display (round 240×240) -----
#define DISPLAY_WIDTH           240
#define DISPLAY_HEIGHT          240
#define DISPLAY_INTERVAL_MS     500

// ----- Shake detection -----
#define SHAKE_THRESHOLD         2.5f
#define SHAKE_DEBOUNCE_MS       800

// ----- LEDs -----
#define LED_BRIGHTNESS          200
#define LED_COUNT               5

// ----- Touch buttons (carrier pad indices) -----
#define TOUCH_INDEX_SHAKE_MODE  TOUCH3
#define TOUCH_INDEX_LED_SHOW    TOUCH4

// ----- Carrier -----
// Set to 1 when using the plastic case (touch calibration)
#define CARRIER_USE_CASE        0

// ----- WiFi (only when USE_WIFI is 1 and secrets.h is filled in) -----
#define USE_WIFI                0
#define WIFI_CONNECT_ATTEMPTS   10
#define WIFI_CONNECT_RETRY_MS   500

// ----- Timing -----
#define STARTUP_SPLASH_DELAY_MS 1500
#define LOOP_DELAY_MS           30

#endif /* CONFIG_H */
