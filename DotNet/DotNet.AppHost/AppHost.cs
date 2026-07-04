var builder = DistributedApplication.CreateBuilder(args);

var cache = builder.AddRedis("cache");

var apiService = builder.AddProject<Projects.DotNet_ApiService>("apiservice")
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
