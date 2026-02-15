# Oplà IoT Carrier – Demo

Demo-projekt til MKR WiFi 1010 + MKR IoT Carrier. Koden er opdelt så I kan vise eleverne god struktur.

## Filstruktur (PlatformIO)

Dette er den anbefalede mappestruktur for et PlatformIO-projekt. Hver mappe har ét formål:

| Mappe | Formål | I dette projekt |
|-------|--------|------------------|
| **`src/`** | Kildekode (`.cpp`, `.c`). Alt der skal kompileres og linkes, inkl. `main.cpp`. PlatformIO kompilerer automatisk alle filer her. | `main.cpp`, `display_utils.cpp`, `led_utils.cpp`, `temp_color.cpp`, `secrets.h` (+ `secrets.h.example`). |
| **`include/`** | Header-filer (`.h`) der skal være fælles for flere kildefiler. PlatformIO tilføjer automatisk denne mappe til "include path", så `#include "config.h"` finder `include/config.h`. | `config.h`, `display_utils.h`, `led_utils.h`, `temp_color.h`. |
| **`lib/`** | **Projektspecifikke** biblioteker – kode der kun bruges i dette projekt og lægges i egne undermapper (fx `lib/min_io/min_io.cpp`). Ydre biblioteker (Arduino_MKRIoTCarrier, WiFiNINA osv.) håndteres i stedet via `platformio.ini` → `lib_deps`. | Tom (vi bruger kun `lib_deps`). Mappen kan **fjernes** hvis I aldrig vil have lokale biblioteker her. |
| **`test/`** | Unit tests til PlatformIO Test Runner (`pio test -e native`). Her lægges testkode der kører på host eller på enheden. | `test_temp_color.cpp` – Unity-tests for temperatur→RGB-logikken. |

**Kort sagt:**  
- **`src/`** og **`include/`** er kerne-strukturen: kildekode i `src/`, fælles headers i `include/`.  
- **`lib/`** er valgfri (tom her). **`test/`** bruges til unit tests – I dette projekt til demo af `temp_color`-logik.

### Skal display_utils og led_utils ligge i `src/` eller `lib/`?

Begge er best practice – det er et valg:

| Placering | Hvornår | Fordele |
|-----------|--------|---------|
| **`src/`** (som nu) | Koden er **del af selve programmet** og bruges kun her. | Simpelt: alt kildekode samlet, færre mapper, nemt at finde. |
| **`lib/`** | I vil tydeligt markere modulerne som **små “biblioteker”** (én mappe per modul, fx `lib/display_utils/`, `lib/led_utils/`). | Klar adskillelse, matcher PlatformIOs lib-mønster; godt hvis I senere vil genbruge modulet i et andet projekt. |

I dette projekt ligger `display_utils` og `led_utils` i **`src/`** for enkelhed. De *kunne* flyttes til fx `lib/display_utils/` og `lib/led_utils/` (med hver sin `.h` og `.cpp` i undermappen), hvis I vil vise lib-strukturen. Funktionaliteten er den samme.

## Opdeling af kode

| Fil | Formål |
|-----|--------|
| **`include/config.h`** | Indstillinger der **må** versioneres. Ingen hemmeligheder. |
| **`src/secrets.h`** | WiFi-kodeord osv. **Committes ikke** – står i `.gitignore`. |
| **`src/secrets.h.example`** | Skabelon til `secrets.h`. Kopiér til `secrets.h` og udfyld. |
| **`include/display_utils.h`** + **`src/display_utils.cpp`** | Alt der vises på skærmen: centrering af tekst, opdatering af demo-skærm, startskærm. |
| **`include/led_utils.h`** + **`src/led_utils.cpp`** | Styring af de 5 RGB-LEDs: regnbue, temperatur-farve, slukket, quick-show. |
| **`include/temp_color.h`** + **`src/temp_color.cpp`** | Ren logik: temperatur → RGB (ingen Arduino). Bruges af `led_utils` og af **unit tests**. |
| **`src/main.cpp`** | Kun **setup**, **loop** og logik (touch, sensorer, gesture). Inkluderer de andre moduler. |

**Hvorfor .h + .cpp?** Headeren (`.h`) erklarer *hvad* modulet tilbyder (funktionssignaturer). Implementeringen (`.cpp`) indeholder *hvordan* det virker. Så kan `main.cpp` holde sig kort og overskuelig, og display/LED-kode kan genbruges eller testes for sig.

## Første gang (nye elever / klon af repo)

1. **Kopiér skabelonen:**  
   Kopiér `src/secrets.h.example` til `src/secrets.h`.
2. **Udfyld WiFi (valgfrit):**  
   Åbn `secrets.h` og skriv jeres netværksnavn og adgangskode.  
   Hvis I vil bruge WiFi i demoen, sæt `USE_WIFI` til `1` i `config.h`.
3. **Byg og upload** som normalt med PlatformIO.

## Hvorfor denne struktur?

- **config.h**: Alle kan se og ændre indstillinger; ingen følsomme data.
- **secrets.h**: Står i `.gitignore`, så WiFi-kodeord aldrig ryger på Git.
- **secrets.h.example**: Gør det nemt at komme i gang uden at committe rigtige passwords.
- **display_utils / led_utils**: Klassisk C++-opdeling: én .h med erklæringer, én .cpp med kode. `main.cpp` bliver kort og logikken let at følge.
- **temp_color**: Ren logik uden hardware – bruges både af `led_utils` og af unit tests, så I kan teste uden board.
- **test/**: Unity-tests kører på PC med `pio test -e native` og viser at “ren logik” kan testes uden upload.

## Byg og upload

```bash
cd Arduino
pio run -t upload
```

Eller brug "Upload" i PlatformIO / Cursor.

## Kør unit tests (på PC)

Projektet har simple Unity-tests som demo. De tester kun **temp_color** (temperatur → RGB), fordi den kode ikke bruger Arduino og kan køre på din computer:

```bash
cd Arduino
pio test -e native
```

- **`test/test_temp_color.cpp`** – tre tests: kold temp giver blå, varm giver rød, mellem giver overgang.
- **`[env:native]`** i `platformio.ini` – bygger kun `temp_color.cpp` (resten af `src/` kræver hardware).  
Sådan kan I vise eleverne, at man kan teste “ren logik” uden at uploade til boardet.
