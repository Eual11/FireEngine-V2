#version 330 core

#define MAX_LIGHTS_COUNT 10
struct Light
{
    vec3 ambient_color;
    vec3 diffuse_color;
    vec3 specular_color;
    bool light_active;
    vec3 direction;
    vec3 position;

    float inner_cutoff;
    float outer_cutoff;

    float constant;
    float linear;
    float quadratic;
};
struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};

uniform Light dirLights[MAX_LIGHTS_COUNT];
in vec4 fragPosition;
in vec3 fragNormal;
in vec2 fragTexCoord;
uniform vec3 uViewPos;
uniform Material material;

uniform vec3 ambientLight;

out vec4 FragColor;

//functions to calculate light

vec3 calculateDirectionalLight(Light dirLight, vec3 normal, vec3 viewDir);
void main()
{
    vec3 result = vec3(0.0f);
    vec3 ambient = ambientLight * vec3(texture(material.texture_diffuse1, fragTexCoord));
    vec3 viewDir = normalize(uViewPos - vec3(fragPosition));

    //calculating the cumulative directional lights

    for (int i = 0; i < MAX_LIGHTS_COUNT; i++)
    {
        if (dirLights[i].light_active)
        {
            result += calculateDirectionalLight(dirLights[i], fragNormal, viewDir);
        }
    }
    result += ambient;
    FragColor = vec4(result, 1.0f);
}

vec3 calculateDirectionalLight(Light dirLight, vec3 normal, vec3 viewDir)
{
    vec3 result = vec3(0.0f);
    vec3 sampled_diffuse = vec3(texture(material.texture_diffuse1, fragTexCoord));
    vec3 sampled_specular = vec3(texture(material.texture_specular1, fragTexCoord));

    vec3 light_dir = normalize(-dirLight.direction);
    float diff = max(dot(light_dir, normal), 0.0f);
    vec3 diffuse = diff * dirLight.diffuse_color * sampled_diffuse;
    vec3 ref_vec = reflect(-light_dir, normal);

    float spec = pow(max(dot(ref_vec, viewDir), 0.0f), 64.0f);
    vec3 specular = spec * dirLight.specular_color * sampled_specular;
    specular = vec3(0.0f);
    result = diffuse + specular;
    return result;
}
