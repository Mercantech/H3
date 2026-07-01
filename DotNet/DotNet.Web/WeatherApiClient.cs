namespace DotNet.Web;

public class WeatherApiClient(HttpClient httpClient)
{
    public async Task<WeatherApiResult> FetchWeatherAsync(int maxItems = 10, CancellationToken cancellationToken = default)
    {
        const string path = "/weatherforecast";
        var startedAt = DateTimeOffset.UtcNow;
        var stopwatch = System.Diagnostics.Stopwatch.StartNew();

        using var request = new HttpRequestMessage(HttpMethod.Get, path);

        try
        {
            using var response = await httpClient.SendAsync(
                request,
                HttpCompletionOption.ResponseHeadersRead,
                cancellationToken);

            stopwatch.Stop();
            var body = await response.Content.ReadAsStringAsync(cancellationToken);
            var forecasts = ParseForecasts(body, maxItems);

            return new WeatherApiResult(
                forecasts,
                BuildTrace(
                    path,
                    startedAt,
                    stopwatch.ElapsedMilliseconds,
                    response,
                    body,
                    error: null));
        }
        catch (Exception ex)
        {
            stopwatch.Stop();
            return new WeatherApiResult(
                [],
                BuildTrace(
                    path,
                    startedAt,
                    stopwatch.ElapsedMilliseconds,
                    response: null,
                    body: null,
                    error: ex.Message));
        }
    }

    private static WeatherForecast[] ParseForecasts(string body, int maxItems)
    {
        if (string.IsNullOrWhiteSpace(body))
        {
            return [];
        }

        var forecasts = System.Text.Json.JsonSerializer.Deserialize<WeatherForecast[]>(
            body,
            new System.Text.Json.JsonSerializerOptions
            {
                PropertyNameCaseInsensitive = true,
            });

        return forecasts?.Take(maxItems).ToArray() ?? [];
    }

    private ApiNetworkTrace BuildTrace(
        string path,
        DateTimeOffset startedAt,
        long durationMs,
        HttpResponseMessage? response,
        string? body,
        string? error)
    {
        var baseAddress = httpClient.BaseAddress?.ToString().TrimEnd('/') ?? "(service discovery)";
        var statusCode = response is null ? 0 : (int)response.StatusCode;
        var statusText = response is null
            ? (error is null ? "—" : "Fejl")
            : response.StatusCode.ToString();

        var responseHeaders = response is null
            ? []
            : response.Headers
                .Concat(response.Content.Headers)
                .SelectMany(header => header.Value.Select(v => (header.Key, Value: v)))
                .OrderBy(header => header.Key, StringComparer.OrdinalIgnoreCase)
                .Select(header => $"{header.Key}: {header.Value}")
                .ToArray();

        return new ApiNetworkTrace(
            Id: Guid.NewGuid(),
            StartedAt: startedAt,
            Method: "GET",
            Path: path,
            Url: $"{baseAddress}{path}",
            StatusCode: statusCode,
            StatusText: statusText,
            DurationMs: durationMs,
            ResponseSizeBytes: body?.Length ?? 0,
            ContentType: response?.Content.Headers.ContentType?.ToString() ?? "—",
            ResponseHeaders: responseHeaders,
            ResponsePreview: FormatPreview(body),
            Error: error,
            Success: response?.IsSuccessStatusCode == true && error is null);
    }

    private static string FormatPreview(string? body)
    {
        if (string.IsNullOrWhiteSpace(body))
        {
            return "(tom response)";
        }

        const int maxLength = 1200;
        return body.Length <= maxLength
            ? body
            : $"{body[..maxLength]}…";
    }
}

public record WeatherApiResult(WeatherForecast[] Forecasts, ApiNetworkTrace Trace);

public record ApiNetworkTrace(
    Guid Id,
    DateTimeOffset StartedAt,
    string Method,
    string Path,
    string Url,
    int StatusCode,
    string StatusText,
    long DurationMs,
    int ResponseSizeBytes,
    string ContentType,
    string[] ResponseHeaders,
    string ResponsePreview,
    string? Error,
    bool Success);

public record WeatherForecast(DateOnly Date, int TemperatureC, string? Summary)
{
    public int TemperatureF => 32 + (int)(TemperatureC / 0.5556);
}
