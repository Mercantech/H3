using Infisical.Sdk;
using Infisical.Sdk.Model;
using Microsoft.AspNetCore.Mvc;

namespace DotNet.ApiService.Controllers
{
    [ApiController]
    [Route("debug")]
    public sealed class DebugController : ControllerBase
    {
        private const string FixedEnvironmentSlug = "debug";
        private const string FixedSecretPath = "/";
        private readonly IConfiguration _configuration;
        private readonly IHostEnvironment _environment;
        private readonly ILogger<DebugController> _logger;

        public DebugController(IConfiguration configuration, IHostEnvironment environment, ILogger<DebugController> logger)
        {
            _configuration = configuration;
            _environment = environment;
            _logger = logger;
        }

        [HttpGet("variables")]
        [ProducesResponseType(StatusCodes.Status200OK)]
        [ProducesResponseType(StatusCodes.Status400BadRequest)]
        [ProducesResponseType(StatusCodes.Status404NotFound)]
        public async Task<IActionResult> GetVariables()
        {
            if (!_environment.IsDevelopment())
            {
                return NotFound();
            }

            var clientId = _configuration["Infisical:ClientId"];
            var clientSecret = _configuration["Infisical:ClientSecret"];
            var projectId = _configuration["Infisical:ProjectId"];
            var hostUri = _configuration["Infisical:HostUri"];

            var missing = new List<string>();
            if (string.IsNullOrWhiteSpace(clientId)) missing.Add("Infisical:ClientId");
            if (string.IsNullOrWhiteSpace(clientSecret)) missing.Add("Infisical:ClientSecret");
            if (string.IsNullOrWhiteSpace(projectId)) missing.Add("Infisical:ProjectId");

            if (missing.Count > 0)
            {
                _logger.LogWarning(
                    "Infisical debug endpoint mangler konfiguration: {MissingFields}",
                    string.Join(", ", missing)
                );

                return BadRequest(new
                {
                    Message = "Infisical credentials/config mangler.",
                    Missing = missing
                });
            }

            var requiredClientId = clientId!;
            var requiredClientSecret = clientSecret!;
            var requiredProjectId = projectId!;

            var settingsBuilder = new InfisicalSdkSettingsBuilder();
            if (!string.IsNullOrWhiteSpace(hostUri))
            {
                settingsBuilder.WithHostUri(hostUri);
            }

            var client = new InfisicalClient(settingsBuilder.Build());
            _ = await client.Auth().UniversalAuth().LoginAsync(requiredClientId, requiredClientSecret);

            var secrets = await client.Secrets().ListAsync(new ListSecretsOptions
            {
                ProjectId = requiredProjectId,
                EnvironmentSlug = FixedEnvironmentSlug,
                SecretPath = FixedSecretPath,
                Recursive = true,
                ExpandSecretReferences = true,
                ViewSecretValue = true
            });

            var variables = secrets
                .Where(secret => !string.IsNullOrWhiteSpace(secret.SecretKey))
                .OrderBy(secret => secret.SecretKey, StringComparer.OrdinalIgnoreCase)
                .ToDictionary(
                    secret => secret.SecretKey,
                    secret => secret.SecretValue
                );

            return Ok(new
            {
                EnvironmentSlug = FixedEnvironmentSlug,
                SecretPath = FixedSecretPath,
                Count = variables.Count,
                Variables = variables
            });
        }
    }
}
