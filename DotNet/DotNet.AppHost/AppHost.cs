var builder = DistributedApplication.CreateBuilder(args);

var cache = builder.AddRedis("cache")
    .WithRedisInsight();

var postgres = builder.AddPostgres("postgres")
    .WithDataVolume()
    .WithPgWeb();

var db = postgres.AddDatabase("h3db");

var apiService = builder.AddProject<Projects.DotNet_ApiService>("apiservice")
    .WithReference(cache)
    .WaitFor(cache)
    .WithReference(db)
    .WaitFor(db)
    // Aspire 9.3+ defaults to HTTPS when available; CI runners do not trust dev certs.
    .WithHttpHealthCheck(endpointName: "http", path: "/health");

builder.AddProject<Projects.DotNet_Web>("webfrontend")
    .WithExternalHttpEndpoints()
    .WithHttpHealthCheck(endpointName: "http", path: "/health")
    .WithReference(cache)
    .WaitFor(cache)
    .WithReference(apiService)
    .WaitFor(apiService);

builder.Build().Run();
