#version 330 core
#define MAX_LIGHTS_COUNT 10
#define PI 3.1415926535897
struct Light
{
    vec3 ambient_color;
    vec3 diffuse_color;
    vec3 specular_color;
    bool light_active;
    vec3 direction;
    vec3 position;

    float intensity;

    float inner_cutoff;
    float outer_cutoff;

    float constant;
    float linear;
    float quadratic;
};
struct Material
{
    //we might use this...
    sampler2D texture_albedo1; //albedo
    sampler2D texture_normal1; //normal map
    sampler2D texture_metalic1; //metalic map
    sampler2D texture_ao1; //ambient_occulusion map
    sampler2D texture_roughness1; //roughness map

    //flags if there is a texture map is bound for each material property
    bool albedo_bound;
    bool normal_bound;
    bool roughness_bound;
    bool ao_bound;
    bool metalic_bound;

    vec3 albedo;
    float metalic;
    float roughness;
};

in vec4 fragPosition;
in vec3 fragNormal;
in vec2 fragTexCoord;
in mat3 TBN;

uniform bool enableNormalMapping;

//fixed number of lights
//TODO: maybe deffered rendering
uniform Light dirLights[MAX_LIGHTS_COUNT];
uniform Light pointLights[MAX_LIGHTS_COUNT];
uniform Light spotLights[MAX_LIGHTS_COUNT];

uniform vec3 uViewPos;
uniform Material material;

uniform vec3 ambientLight;
uniform float ambientIntensity;

uniform bool gammaCorrect;

out vec4 FragColor;

vec3 calculateDirectionalLight(Light dirLight, vec3 normal, vec3 viewDir);
vec3 calculatePointLight(Light pointLight, vec3 normal, vec3 viewDir);
vec3 calculateSpotLight(Light spotLight, vec3 normal, vec3 viewDir);

vec3 BRDF_CookTorrence(vec3 normal, vec3 light_dir, vec3 viewDir);
vec3 F(vec3 F0, vec3 V, vec3 H);
float Gsub(vec3 N, vec3 X, float a);
float G(vec3 N, vec3 V, vec3 L, float a);
float D(vec3 N, vec3 H, float a);

//Default refelectance

void main()
{
    vec3 result = vec3(0.0f);
    vec3 albedo;
    vec3 ambient = ambientIntensity * ambientLight * vec3(0.0);
    vec3 viewDir = normalize(uViewPos - vec3(fragPosition));

    vec3 normal;
    normal = fragNormal;
    if (material.normal_bound && enableNormalMapping)
    {
        normal = texture(material.texture_normal1, fragTexCoord).rgb;
        normal = normalize(TBN * (normal * 2.0f - 1.0f));
    }

    //calculatig the cumulative directional lighting

    for (int i = 0; i < MAX_LIGHTS_COUNT; i++)
    {
        if (dirLights[i].light_active)
        {
            result += calculateDirectionalLight(dirLights[i], normal, viewDir);
        }
    }

    result += ambient;
    if (gammaCorrect)
        result = vec3(pow(result.x, 2.2), pow(result.y, 2.2), pow(result.z, 2.2));

    FragColor = vec4(result, 1.0f);
}

vec3 calculateDirectionalLight(Light dirLight, vec3 normal, vec3 viewDir)
{
    vec3 result = vec3(0.0);
    vec3 light_dir = normalize(-dirLight.direction);
    vec3 BRDF = BRDF_CookTorrence(normal, light_dir, viewDir);
    float diff = max(dot(light_dir, normal), 0.0001);
    result = BRDF * dirLight.diffuse_color * diff * dirLight.intensity;

    return result;
}

vec3 F(vec3 F0, vec3 V, vec3 H)
{
    float thetaPI = pow((1.0 - max(dot(V, H), 0.0005)), 5.0);
    return F0 + (1.0 - F0) * (thetaPI);
}
float D(vec3 N, vec3 H, float a)
{
    float a2 = a * a;
    float NdotH2 = pow(max(dot(N, H), 0.0005), 2.0);

    return a2 / max(0.000001, (PI * (pow((NdotH2) * (a2 - 1) + 1, 2.0))));
}
float Gsub(vec3 N, vec3 X, float a)
{
    float k = (a * a) / 2.0;
    float NdotX = max(dot(N, X), 0.0005);
    return NdotX / ((NdotX) * (1 - k) + k);
}
float G(vec3 N, vec3 V, vec3 L, float a)
{
    return Gsub(N, V, a) * Gsub(N, L, a);
}
vec3 BRDF_CookTorrence(vec3 normal, vec3 light_dir, vec3 viewDir)
{
    vec3 half_vector = normalize(viewDir + light_dir);

    //hard coded material properties
    float roughness = material.roughness;

    //HACK:
    roughness = 0.1;
    vec3 albedo = material.diffuse_color;
    albedo = vec3(1.0, 0.0, 0.0);
    float metalic = material.metalic;
    metalic = 0.1;

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metalic);
    vec3 Ks = F(F0, viewDir, half_vector);
    vec3 Kd = 1.0 - Ks;

    vec3 lambert = albedo / PI;

    float cookTorrenceDenom = 4.0 * dot(viewDir, normal) * dot(light_dir, normal);
    cookTorrenceDenom = max(0.0001, cookTorrenceDenom);

    vec3 cookTorrence = (D(normal, half_vector, roughness) * G(normal, viewDir, light_dir, roughness) * Ks) / cookTorrenceDenom;

    lambert *= Kd;

    return lambert + cookTorrence;
}
