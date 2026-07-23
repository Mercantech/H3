# [GRUPPENAVN] — H3 IoT-projekt

> **Dette er jeres groups README.** Når I har forket repoet, skal I erstatte titel, beskrivelse, links og gruppeinfo nedenfor. Slet denne infoboks, når dokumentet er jeres eget.

## Om projektet

**Kort beskrivelse (2–4 sætninger):**

[Beskriv jeres IoT-løsning: Hvad måler/styrer Arduino-enheden? Hvad viser dashboardet? Hvem er målgruppen?]

**Eksempel fra forløbet:** Arduino Oplà sender miljødata via Wi-Fi til et .NET API, som gemmer og eksponerer data til et Blazor-dashboard med live-visning.

| | |
|---|---|
| **Gruppe** | [Gruppe 1–8] |
| **Medlemmer** | [Navn 1], [Navn 2], [Navn 3] |
| **Undervisere** | Kasper & Mathias |
| **Forløb** | H3 2026 (okt–dec) |
| **Live URL** | [https://h3-web.mercantec.tech](https://h3-web.mercantec.tech) *(eller jeres egen subdomæne-URL)* |
| **Notion-plan** | [mercantec.notion.site/h3-2026](https://mercantec.notion.site/h3-2026) |

## Arkitektur

```
[Arduino Oplà]  --Wi-Fi-->  [.NET API]  <--->  [Database/Redis]
                                ^
                                |
                          [Blazor Web]
```

Tilpas diagrammet til jeres endelige løsning (fx MQTT, flere sensorer, ekstra services).

## Repo-struktur

| Mappe | Indhold |
|-------|---------|
| `Arduino/` | PlatformIO-projekt til MKR WiFi 1010 + IoT Carrier |
| `DotNet/` | .NET Aspire-stack: API, Blazor-web, Redis, Docker |
| `Notes/` | Obsidian-noter til dokumentation og gruppearbejde |
| `.obsidian/` | Obsidian-konfiguration (plugins m.m.) |

Detaljeret opsætning:

- **Arduino:** se [`Arduino/README.md`](Arduino/README.md)
- **.NET / Docker:** se [`DotNet/README.md`](DotNet/README.md)

## Kom i gang

### 1. Klon / fork

```powershell
git clone https://github.com/[JERES-ORG]/[JERES-REPO].git
cd [JERES-REPO]
```

### 2. Arduino (hardware)

```powershell
cd Arduino
copy src\secrets.h.example src\secrets.h
# Udfyld WiFi i secrets.h
pio run -t upload
```

### 3. .NET lokalt (Aspire)

```powershell
cd DotNet
dotnet restore DotNet.slnx
dotnet run --project DotNet.AppHost
```

Aspire starter **web**, **api** og **redis** og viser endpoints i dashboard/log.

### 4. Docker / deploy (Dokploy)

```powershell
cd DotNet
copy .env.example .env
# Udfyld APP_DOMAIN og evt. Infisical-credentials
docker compose up -d --build
```

Standard-URL i skabelonen: `h3-web.mercantec.tech` — skift `APP_DOMAIN` i `.env` hvis I får eget subdomæne.
Admin-UI’er: `h3-pgweb.mercantec.tech` (Postgres) og `h3-redis.mercantec.tech` (Redis).

## Hvad skal I tilpasse i skabelonen?

Brug denne tjekliste efter fork:

- [ ] **Denne README** — projektbeskrivelse, gruppe, links, arkitektur
- [ ] **`DotNet/DotNet.Web/Components/Pages/Home.razor`** — forløbs-/projekt-infographic
- [ ] **`DotNet/DotNet.Web/Components/Layout/NavMenu.razor`** — titel og navigation
- [ ] **`DotNet/docker-compose.yml` / `.env`** — deploy-URL og secrets
- [ ] **`Arduino/src/`** — jeres sensor- og netværkslogik
- [ ] **`DotNet/DotNet.ApiService/`** — jeres rigtige API-endpoints (erstat demo `/weatherforecast`)
- [ ] **`DotNet/DotNet.Web/`** — dashboard der viser jeres IoT-data
- [ ] **`Notes/`** — dokumentation, sprint-noter, beslutninger

## Secrets og konfiguration

| Hvad | Hvor | Committes? |
|------|------|------------|
| WiFi/adgangskoder (Arduino) | `Arduino/src/secrets.h` | Nej — brug `secrets.h.example` |
| Infisical / API-secrets | Env vars eller Infisical CLI | Nej |
| Ikke-hemmelig config | `appsettings.example.json` → kopiér lokalt | Eksempelfiler ja, rigtige secrets nej |

Lokal Infisical:

```powershell
infisical run -- dotnet run --project DotNet.AppHost
```

## Tests

```powershell
cd DotNet
dotnet test DotNet.Tests
```

```powershell
cd Arduino
pio test -e native
```

## MVP og udvidelser

**Vores MVP (minimum):**

- [ ] [Fx: Enhed kan registreres i dashboard]
- [ ] [Fx: Live temperatur/fugt vises]
- [ ] [Fx: API deployet og tilgængeligt]

**Stretch / nice-to-have:**

- [ ] [Fx: Historik grafer]
- [ ] [Fx: Notifikationer ved threshold]
- [ ] [Fx: To-vejs styring fra web til Arduino]

## Links

| Ressource | URL |
|-----------|-----|
| Forløbsplan (Notion) | [https://mercantec.notion.site/h3-2026](https://mercantec.notion.site/h3-2026) |
| Mercantech skabelon (upstream) | [https://github.com/Mercantech/H3](https://github.com/Mercantech/H3) |
| Jeres GitHub-repo | [https://github.com/...](https://github.com/) |
| Live app | [https://...](https://) |
| Figma / design *(valgfrit)* | [link] |

---

*Skabelon fra Mercantec H3 2026 — fork og gør den til jeres egen.*
