/*
 * config.h – Projektkonfiguration (ingen hemmeligheder)
 *
 * Her samles alle indstillinger, der kan deles og versioneres.
 * WiFi-adgangskoder m.m. hører til i secrets.h (som er i .gitignore).
 */

#ifndef CONFIG_H
#define CONFIG_H

// ----- Seriel kommunikation -----
#define SERIAL_BAUD 9600

// ----- Display (rund skærm 240×240) -----
#define DISPLAY_W           240
#define DISPLAY_H           240

// ----- Display-opdatering -----
#define DISPLAY_INTERVAL_MS 500

// ----- Ryst-detektor -----
#define SHAKE_THRESHOLD      2.5f
#define SHAKE_DEBOUNCE_MS    800

// ----- LEDs -----
#define LED_BRIGHTNESS  200
#define LED_COUNT       5

// ----- Oplà carrier -----
// Sæt til 1 hvis du bruger den medfølgende plastik-kasse (kalibrerer touch)
#define CARRIER_USE_CASE 0

// ----- WiFi (bruges kun hvis USE_WIFI er 1 og secrets.h er udfyldt) -----
#define USE_WIFI 0

#endif /* CONFIG_H */
