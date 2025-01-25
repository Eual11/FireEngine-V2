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
    sampler2D texture_diffuse1; //albedo
    sampler2D texture_normal1; //normal map
    sampler2D texture_metalic; //metalic map
    sampler2D texture_ao; //ambient_occulusion map
    sampler2D texture_roughness; //roughness map

    //flags if there is a texture map is bound for each material property
    bool diffuse_bound;
    bool normal_bound;
    bool roughness_bound;
    bool ao_bound;
    bool metalic_bound;

    vec3 diffuse_color; //albedo
    float metalic;
    float roughness;
    float ao;
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

vec3 F(vec3 F0, vec3 V, vec3 H);

//Default refelectance

vec3 F0 = vec3(0.04);
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
    vec3 albedo = vec3(1.0, 0.0, 0.0);
    vec3 light_dir = normalize(-dirLight.direction);
    vec3 half_vector = normalize(viewDir + light_dir);

    //hard coded material properties

    float roughness = 0.1;
    float alpha = roughness * roughness;

    float metalic = 0.0;

    F0 = mix(F0, albedo, metalic);
    vec3 Ks = F(F0, viewDir, half_vector);
    vec3 Kd = 1.0 - Ks;

    vec3 lambert = albedo / PI;

    float cookTorrenceDenom = 4.0 * dot(viewDir, normal) * dot(light_dir, normal);
    cookTorrenceDenom = max(0.0001, cookTorrenceDenom);
    lambert *= Kd;

    float diff = max(dot(light_dir, normal), 0.0);

    vec3 diffuse = dirLight.intensity * diff * dirLight.diffuse_color * albedo;
    result += vec3(cookTorrenceDenom);
    return result;
}

vec3 F(vec3 F0, vec3 V, vec3 H)
{
    float thetaPI = pow((1.0 - max(dot(V, H), 0.005)), 5.0);
    return F0 + (1.0 - F0) * (thetaPI);
}
