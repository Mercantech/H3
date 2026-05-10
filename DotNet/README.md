# DotNet setup

Dette er en .NET Aspire-losning med:

- `DotNet.AppHost` (orchestrering af hele stacken)
- `DotNet.Web` (frontend i ASP.NET Core/Blazor)
- `DotNet.ApiService` (API service)
- `DotNet.ServiceDefaults` (faelles telemetry, health checks, resilience, service discovery)
- `DotNet.Tests` (integrationstest via Aspire testing)

## Krav

- .NET SDK 10 (`net10.0`)
- Docker Desktop (bruges af Aspire til Redis container)
- Valgfrit: Visual Studio 2022 / Rider / VS Code

Tjek installation:

```powershell
dotnet --info
docker --version
```

## Hurtig start

Fra `DotNet/`:

```powershell
dotnet restore DotNet.slnx
dotnet build DotNet.slnx
dotnet run --project DotNet.AppHost
```

`DotNet.AppHost` starter de underliggende services (web, api, redis) og viser endpoints i Aspire dashboard/log output.

## Projektstruktur

- `DotNet.AppHost/AppHost.cs`
  - Starter Redis resource (`cache`)
  - Starter `apiservice` og `webfrontend`
  - Konfigurerer health checks mellem services
- `DotNet.Web/Program.cs`
  - Bruger `AddServiceDefaults()`
  - Bruger Redis output cache via `AddRedisOutputCache("cache")`
  - Kalder API via service discovery (`https+http://apiservice`)
- `DotNet.ApiService/Program.cs`
  - Eksponerer `/weatherforecast`
  - Har OpenAPI i development
  - Mapper standard health endpoints via `MapDefaultEndpoints()`
- `DotNet.ServiceDefaults/Extensions.cs`
  - OpenTelemetry, service discovery, standard resilience handler
  - Health endpoints i development (`/health`, `/alive`)

## Konfiguration

### Appsettings

Der ligger en eksempel-fil i:

- `DotNet.ApiService/appsettings.example.json`

Hvis du skal override lokalt:

```powershell
copy DotNet.ApiService\appsettings.example.json DotNet.ApiService\appsettings.Development.json
```

### Infisical (Machine Identity)

`DotNet.ApiService` kan hente secrets fra Infisical ved startup.

API'et loader Infisical secrets, hvis disse noegler er sat:

- `Infisical:ClientId`
- `Infisical:ClientSecret`
- `Infisical:ProjectId`
- `Infisical:EnvironmentSlug`
- `Infisical:SecretPath` (valgfri, default `/`)
- `Infisical:HostUri` (valgfri, default `https://app.infisical.com`)

I praksis saettes de normalt som environment variables:

```powershell
Infisical__ClientId
Infisical__ClientSecret
Infisical__ProjectId
Infisical__EnvironmentSlug
Infisical__SecretPath
Infisical__HostUri
```

Infisical keys med `__` bliver mappet til nested .NET config keys, fx:

- `ConnectionStrings__Main` -> `ConnectionStrings:Main`
- `Jwt__SigningKey` -> `Jwt:SigningKey`

#### Lokal udvikling

Koer med Infisical CLI:

```powershell
infisical run -- dotnet run --project DotNet.AppHost
```

### Swagger og debug endpoint (ApiService)

I development er Swagger UI aktiveret for `DotNet.ApiService`:

- Swagger UI: `/swagger`
- OpenAPI JSON: `/openapi/v1.json`

Der er ogsaa et debug-endpoint til hurtig validering af runtime-konfiguration:

- `GET /debug/variables` (henter direkte fra Infisical platformen)
- Endpointet bruger fast `EnvironmentSlug=debug` og `SecretPath=/debug`

Endpointet er kun aktivt i development og returnerer rae secret values til debug.

#### Dokploy server

- Gem Machine Identity credentials som secrets i Dokploy (ikke i repo).
- Injicer credentials som env vars til API/AppHost processen.
- Start appen med Infisical injection, saa runtime henter secrets ved opstart.

#### CI/CD

- Gem Machine Identity credentials som CI secrets.
- Koer build/test/deploy steps med Infisical-injicerede env vars.
- Undgaa at printe secrets i logs.

### User Secrets (AppHost)

`DotNet.AppHost` har `UserSecretsId` i projektfilen. Brug secrets til lokal, sensitiv konfiguration:

```powershell
dotnet user-secrets --project DotNet.AppHost set "Some:Key" "SomeValue"
```

### Telemetry eksport (valgfrit)

Hvis `OTEL_EXPORTER_OTLP_ENDPOINT` er sat, sender services telemetry via OTLP (se `DotNet.ServiceDefaults/Extensions.cs`).

## Udviklingskommandoer

Kores fra `DotNet/`:

```powershell
# Build alt
dotnet build DotNet.slnx

# Kør hele stacken via Aspire
dotnet run --project DotNet.AppHost

# Kør kun API
dotnet run --project DotNet.ApiService

# Kør kun Web
dotnet run --project DotNet.Web

# Tests
dotnet test DotNet.Tests
```

## Health checks

I development mapes:

- `/health` (readiness)
- `/alive` (liveness)

Disse endpoints kommer fra `DotNet.ServiceDefaults`.

## Fejlfinding

- **Port eller endpoint vises ikke**
  - Start altid via `DotNet.AppHost` for fuld orchestration og korrekt service discovery.
- **Redis relaterede fejl**
  - Bekraeft at Docker Desktop korer.
- **Service discovery fejl mellem Web og API**
  - Verificer at begge services er startet af AppHost.
- **Ingen OpenAPI endpoint**
  - OpenAPI mapes kun i development for `DotNet.ApiService`.
