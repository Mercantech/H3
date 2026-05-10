using Infisical.Sdk;
using Infisical.Sdk.Model;

var builder = WebApplication.CreateBuilder(args);
await InfisicalConfigurationLoader.TryLoadSecretsAsync(builder.Configuration);

// Add service defaults & Aspire client integrations.
builder.AddServiceDefaults();

// Add services to the container.
builder.Services.AddProblemDetails();

// Learn more about configuring OpenAPI at https://aka.ms/aspnet/openapi
builder.Services.AddOpenApi();
builder.Services.AddEndpointsApiExplorer();
builder.Services.AddControllers();
builder.Services.AddSwaggerGen();

var app = builder.Build();

// Configure the HTTP request pipeline.
app.UseExceptionHandler();

if (app.Environment.IsDevelopment())
{
    app.MapOpenApi();
    app.UseSwagger();
    app.UseSwaggerUI();
}

string[] summaries = ["Freezing", "Bracing", "Chilly", "Cool", "Mild", "Warm", "Balmy", "Hot", "Sweltering", "Scorching"];

app.MapGet("/weatherforecast", () =>
{
    var forecast = Enumerable.Range(1, 5).Select(index =>
        new WeatherForecast
        (
            DateOnly.FromDateTime(DateTime.Now.AddDays(index)),
            Random.Shared.Next(-20, 55),
            summaries[Random.Shared.Next(summaries.Length)]
        ))
        .ToArray();
    return forecast;
})
.WithName("GetWeatherForecast");

app.MapControllers();
app.MapDefaultEndpoints();

app.Run();

record WeatherForecast(DateOnly Date, int TemperatureC, string? Summary)
{
    public int TemperatureF => 32 + (int)(TemperatureC / 0.5556);
}

static class InfisicalConfigurationLoader
{
    public static async Task TryLoadSecretsAsync(ConfigurationManager configuration)
    {
        var clientId = configuration["Infisical:ClientId"];
        var clientSecret = configuration["Infisical:ClientSecret"];
        var projectId = configuration["Infisical:ProjectId"];
        var environmentSlug = configuration["Infisical:EnvironmentSlug"];
        var secretPath = configuration["Infisical:SecretPath"] ?? "/";
        var hostUri = configuration["Infisical:HostUri"];

        if (string.IsNullOrWhiteSpace(clientId)
            || string.IsNullOrWhiteSpace(clientSecret)
            || string.IsNullOrWhiteSpace(projectId)
            || string.IsNullOrWhiteSpace(environmentSlug))
        {
            return;
        }

        var settingsBuilder = new InfisicalSdkSettingsBuilder();
        if (!string.IsNullOrWhiteSpace(hostUri))
        {
            settingsBuilder.WithHostUri(hostUri);
        }

        var client = new InfisicalClient(settingsBuilder.Build());
        _ = await client.Auth().UniversalAuth().LoginAsync(clientId, clientSecret);

        var secrets = await client.Secrets().ListAsync(new ListSecretsOptions
        {
            ProjectId = projectId,
            EnvironmentSlug = environmentSlug,
            SecretPath = secretPath,
            Recursive = true,
            ExpandSecretReferences = true,
            ViewSecretValue = true,
        });

        var overrides = new Dictionary<string, string?>(StringComparer.OrdinalIgnoreCase);
        foreach (var secret in secrets)
        {
            if (string.IsNullOrWhiteSpace(secret.SecretKey))
            {
                continue;
            }

            // Infisical keys can use __, but .NET in-memory provider expects : for nesting.
            var normalizedKey = secret.SecretKey.Replace("__", ":");
            overrides[normalizedKey] = secret.SecretValue;
        }

        if (overrides.Count > 0)
        {
            configuration.AddInMemoryCollection(overrides);
        }
    }
}
