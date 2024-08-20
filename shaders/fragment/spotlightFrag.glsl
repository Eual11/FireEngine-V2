#version 330 core

struct Material
{
    //this represents the different color/reflection and absorbition of the material under different condition

    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};
//Light Properties, in this model even light sources have ambient, diffuse and specular

struct SpotLight
{
    vec3 ambient;
    vec3 specular;
    vec3 diffuse;

    float shineness;

    vec3 direction;
    vec3 position;
    float cutoff;
};
in vec3 fragNormal;
in vec4 fragPosition;
in vec2 fragTexCoord;
uniform SpotLight light;
uniform vec3 uViewPos;
uniform Material material;
uniform float uTime;

out vec4 FragColor;
void main()
{

    //calculating ambient lighting
    vec3 result = vec3(0.0);

    vec3 light_dir = normalize(light.position - fragPosition.xyz);
    vec3 ambient = light.ambient * texture(material.texture_diffuse1, fragTexCoord).rgb;

    //diffuse
    float diff = max(dot(light_dir, fragNormal), 0.0f);
    vec3 diffuse = light.diffuse * (texture(material.texture_diffuse1, fragTexCoord).rgb * diff);
    vec3 viewDir = normalize(uViewPos - fragPosition.xyz);
    vec3 R = reflect(-light.direction, fragNormal);
    float spec = max(dot(viewDir, R), 0);
    spec = pow(spec, material.shineness);
    vec3 specular = light.specular * (texture(material.texture_specular1, fragTexCoord).rgb * spec);

    float theta = max(dot(light_dir, normalize(-light.direction)), 0.0f);

    result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0f);
}
