namespace DotNet.Web;

public class HealthApiClient(HttpClient httpClient)
{
    public async Task<HealthProbeResult> CheckApiAsync(CancellationToken cancellationToken = default)
    {
        const string path = "/health";
        var startedAt = DateTimeOffset.UtcNow;
        var stopwatch = System.Diagnostics.Stopwatch.StartNew();

        try
        {
            using var response = await httpClient.GetAsync(path, cancellationToken);
            stopwatch.Stop();
            var body = await response.Content.ReadAsStringAsync(cancellationToken);
            var report = ParseReport(body);

            return new HealthProbeResult(
                Target: "ApiService",
                Path: path,
                Url: BuildUrl(path),
                StartedAt: startedAt,
                DurationMs: stopwatch.ElapsedMilliseconds,
                StatusCode: (int)response.StatusCode,
                StatusText: response.StatusCode.ToString(),
                Success: response.IsSuccessStatusCode,
                Error: response.IsSuccessStatusCode ? null : $"HTTP {(int)response.StatusCode}",
                Report: report,
                RawBody: body);
        }
        catch (Exception ex)
        {
            stopwatch.Stop();
            return new HealthProbeResult(
                Target: "ApiService",
                Path: path,
                Url: BuildUrl(path),
                StartedAt: startedAt,
                DurationMs: stopwatch.ElapsedMilliseconds,
                StatusCode: 0,
                StatusText: "Fejl",
                Success: false,
                Error: ex.Message,
                Report: null,
                RawBody: null);
        }
    }

    private string BuildUrl(string path)
    {
        var baseAddress = httpClient.BaseAddress?.ToString().TrimEnd('/') ?? "(service discovery)";
        return $"{baseAddress}{path}";
    }

    private static HealthReportDto? ParseReport(string body)
    {
        if (string.IsNullOrWhiteSpace(body))
        {
            return null;
        }

        try
        {
            return System.Text.Json.JsonSerializer.Deserialize<HealthReportDto>(
                body,
                new System.Text.Json.JsonSerializerOptions
                {
                    PropertyNameCaseInsensitive = true,
                });
        }
        catch
        {
            return null;
        }
    }
}

public record HealthProbeResult(
    string Target,
    string Path,
    string Url,
    DateTimeOffset StartedAt,
    long DurationMs,
    int StatusCode,
    string StatusText,
    bool Success,
    string? Error,
    HealthReportDto? Report,
    string? RawBody);

public record HealthReportDto(
    string Status,
    double TotalDurationMs,
    HealthCheckDto[]? Checks);

public record HealthCheckDto(
    string Name,
    string Status,
    double DurationMs,
    string? Description,
    string? Error,
    string[]? Tags);
