using System.Text.Json;
using Microsoft.AspNetCore.Mvc;
using Npgsql;
using StackExchange.Redis;

namespace DotNet.ApiService.Controllers;

[ApiController]
[Route("demo")]
public sealed class DemoController(
    IConnectionMultiplexer redis,
    NpgsqlDataSource postgres,
    ILogger<DemoController> logger) : ControllerBase
{
    private const string RedisDemoKey = "h3:demo:ping";
    private const string PostgresTable = "demo_pings";

    [HttpPost("seed/redis")]
    [ProducesResponseType(StatusCodes.Status200OK)]
    [ProducesResponseType(StatusCodes.Status500InternalServerError)]
    public async Task<IActionResult> SeedRedis(CancellationToken cancellationToken)
    {
        try
        {
            var payload = new
            {
                message = "H3 Redis dummy data",
                source = "DotNet.ApiService",
                createdAt = DateTimeOffset.UtcNow
            };

            var json = JsonSerializer.Serialize(payload);
            var db = redis.GetDatabase();
            await db.StringSetAsync(RedisDemoKey, json, TimeSpan.FromHours(24));

            var stored = await db.StringGetAsync(RedisDemoKey);

            return Ok(new
            {
                Target = "redis",
                Key = RedisDemoKey,
                TtlHours = 24,
                Written = payload,
                ReadBack = stored.HasValue ? stored.ToString() : null,
                Hint = "Se nøglen i Redis Commander (h3-redis.mercantec.tech)"
            });
        }
        catch (Exception ex)
        {
            logger.LogError(ex, "Kunne ikke skrive dummy data til Redis");
            return Problem(title: "Redis seed fejlede", detail: ex.Message);
        }
    }

    [HttpPost("seed/postgres")]
    [ProducesResponseType(StatusCodes.Status200OK)]
    [ProducesResponseType(StatusCodes.Status500InternalServerError)]
    public async Task<IActionResult> SeedPostgres(CancellationToken cancellationToken)
    {
        try
        {
            await using var connection = await postgres.OpenConnectionAsync(cancellationToken);

            await using (var create = connection.CreateCommand())
            {
                create.CommandText =
                    """
                    CREATE TABLE IF NOT EXISTS demo_pings (
                        id BIGSERIAL PRIMARY KEY,
                        message TEXT NOT NULL,
                        source TEXT NOT NULL,
                        created_at TIMESTAMPTZ NOT NULL DEFAULT NOW()
                    );
                    """;
                await create.ExecuteNonQueryAsync(cancellationToken);
            }

            long id;
            var message = "H3 Postgres dummy data";
            var source = "DotNet.ApiService";
            var createdAt = DateTimeOffset.UtcNow;

            await using (var insert = connection.CreateCommand())
            {
                insert.CommandText =
                    """
                    INSERT INTO demo_pings (message, source, created_at)
                    VALUES (@message, @source, @createdAt)
                    RETURNING id;
                    """;
                insert.Parameters.AddWithValue("message", message);
                insert.Parameters.AddWithValue("source", source);
                insert.Parameters.AddWithValue("createdAt", createdAt);
                id = (long)(await insert.ExecuteScalarAsync(cancellationToken))!;
            }

            object? latest = null;
            await using (var select = connection.CreateCommand())
            {
                select.CommandText =
                    """
                    SELECT id, message, source, created_at
                    FROM demo_pings
                    ORDER BY id DESC
                    LIMIT 5;
                    """;

                var rows = new List<object>();
                await using var reader = await select.ExecuteReaderAsync(cancellationToken);
                while (await reader.ReadAsync(cancellationToken))
                {
                    rows.Add(new
                    {
                        id = reader.GetInt64(0),
                        message = reader.GetString(1),
                        source = reader.GetString(2),
                        createdAt = reader.GetFieldValue<DateTimeOffset>(3)
                    });
                }

                latest = rows;
            }

            return Ok(new
            {
                Target = "postgres",
                Table = PostgresTable,
                InsertedId = id,
                Written = new { id, message, source, createdAt },
                LatestRows = latest,
                Hint = "Se tabellen demo_pings i pgweb (h3-pgweb.mercantec.tech)"
            });
        }
        catch (Exception ex)
        {
            logger.LogError(ex, "Kunne ikke skrive dummy data til Postgres");
            return Problem(title: "Postgres seed fejlede", detail: ex.Message);
        }
    }

    [HttpGet("redis")]
    [ProducesResponseType(StatusCodes.Status200OK)]
    [ProducesResponseType(StatusCodes.Status404NotFound)]
    public async Task<IActionResult> GetRedisDemo()
    {
        var value = await redis.GetDatabase().StringGetAsync(RedisDemoKey);
        if (!value.HasValue)
        {
            return NotFound(new { Key = RedisDemoKey, Message = "Ingen dummy data endnu — kør POST /demo/seed/redis" });
        }

        return Ok(new { Key = RedisDemoKey, Value = value.ToString() });
    }

    [HttpGet("postgres")]
    [ProducesResponseType(StatusCodes.Status200OK)]
    public async Task<IActionResult> GetPostgresDemo(CancellationToken cancellationToken)
    {
        await using var connection = await postgres.OpenConnectionAsync(cancellationToken);
        await using var command = connection.CreateCommand();
        command.CommandText =
            """
            SELECT EXISTS (
                SELECT 1
                FROM information_schema.tables
                WHERE table_schema = 'public' AND table_name = 'demo_pings'
            );
            """;

        var exists = (bool)(await command.ExecuteScalarAsync(cancellationToken))!;
        if (!exists)
        {
            return Ok(new { Table = PostgresTable, Rows = Array.Empty<object>(), Message = "Tabellen findes ikke endnu — kør POST /demo/seed/postgres" });
        }

        command.CommandText =
            """
            SELECT id, message, source, created_at
            FROM demo_pings
            ORDER BY id DESC
            LIMIT 20;
            """;

        var rows = new List<object>();
        await using var reader = await command.ExecuteReaderAsync(cancellationToken);
        while (await reader.ReadAsync(cancellationToken))
        {
            rows.Add(new
            {
                id = reader.GetInt64(0),
                message = reader.GetString(1),
                source = reader.GetString(2),
                createdAt = reader.GetFieldValue<DateTimeOffset>(3)
            });
        }

        return Ok(new { Table = PostgresTable, Count = rows.Count, Rows = rows });
    }
}
