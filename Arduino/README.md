# Oplà IoT Carrier – Demo

Demo-projekt til MKR WiFi 1010 + MKR IoT Carrier med tydelig modulopdeling.

## Kodestandard (vigtigt)

- **Variabler, funktioner, filnavne og typer:** altid på **engelsk** (`temperatureCelsius`, `renderButtonLeds`, …)
- **Kommentarer og README:** gerne på dansk til undervisning
- **Display-tekst:** kan være engelsk i skabelonen — tilpas til jeres brugere
- **`main.cpp`:** kun wiring — læs sensorer, kald moduler, ingen tung logik

## Moduler

| Modul | Filer | Ansvar |
|-------|-------|--------|
| **Config** | `include/config.h`, `src/secrets.h` | Indstillinger og credentials |
| **Types** | `include/types.h` | `SensorReading` m.m. |
| **Sensors** | `environment_sensors.*` | Læs temp, fugt, tryk |
| **Display** | `display_renderer.*` | Rund skærm |
| **LEDs** | `led_controller.*` | RGB-ring |
| **Input** | `shake_detector.*`, `gesture_reader.*` | Ryst og gesture |
| **Network** | `wifi_manager.*` | Valgfri WiFi |
| **Logic** | `temperature_color.*` | Ren C++ — testbar på PC |
| **App** | `main.cpp` | `setup()` + `loop()` |

## Filstruktur (PlatformIO)

| Mappe | Formål |
|-------|--------|
| **`src/`** | Kildekode (`.cpp`), inkl. `main.cpp` |
| **`include/`** | Headers (`.h`) — automatisk include path |
| **`lib/`** | Valgfri lokale biblioteker (tom her) |
| **`test/`** | Unity-tests (`pio test -e native`) |

## Første gang

```powershell
cd Arduino
copy src\secrets.h.example src\secrets.h
# Udfyld secrets.h — sæt USE_WIFI til 1 i config.h hvis I vil bruge WiFi
pio run -t upload
```

## Byg og test

```bash
pio run -t upload          # firmware til board
pio test -e native         # unit tests på PC (kun temperature_color)
```

`[env:native]` i `platformio.ini` bygger kun `temperature_color.cpp`, fordi resten kræver Arduino-hardware.

## Udvid projektet

Typisk næste skridt for H3-grupper:

1. Tilføj HTTP/MQTT i `wifi_manager` eller et nyt `api_client`-modul
2. Udvid `SensorReading` og `environment_sensors` med flere målinger
3. Erstat demo-display med jeres eget dashboard-layout i `display_renderer`
4. Skriv flere pure-logic moduler med native tests (som `temperature_color`)
