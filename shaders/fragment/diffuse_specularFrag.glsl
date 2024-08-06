#version 330 core

struct Material
{
    //this represents the different color/reflection and absorbition of the material under different condition

    sampler2D diffuse;
    sampler2D specular;
    float shineness;
};
//Light Properties, in this model even light sources have ambient, diffuse and specular

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
in vec3 Normal;
in vec4 FragPos;
in vec2 TexCoord;
uniform Light light;
uniform vec3 viewPos;
uniform Material material;
uniform sampler2D emission;
uniform float gTime;

out vec4 FragColor;
void main()
{

    //ambient light
    //

    //ambient light is the light that bounce off from the surrounding and lights and illuminates the object

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

    //diffuse light
    //this simulates the light that randomly scatters around the surface of object due to surface irregularity
    //it depends on the angle of the surface

    vec3 light_dir = normalize(light.position - FragPos.xyz);
    float diff = max(dot(light_dir, Normal), 0);
    vec3 diffuse = light.diffuse * (vec3(texture(material.diffuse, TexCoord)) * diff);

    //specular light
    //this simulates the highlights observed in shiny objects when the light's reflection angle aligns with the viewing angle creating a shiny spot

    vec3 viewDir = normalize(viewPos - FragPos.xyz);

    vec3 reflVec = reflect(-light_dir, Normal);

    vec3 emissionVal = vec3(0.0f);

    if (texture(material.specular, TexCoord).rgb == vec3(0.0f))
    {
        vec2 new_text = TexCoord;
        new_text.t = 0.6 * gTime;
        emissionVal = texture(emission, new_text).rgb * 2.0f;
    }
    float spec = pow(max(dot(viewDir, reflVec), 0.0), material.shineness);
    vec3 specular = (spec * vec3(texture(material.specular, TexCoord))) * light.specular;
    vec3 result = (ambient + diffuse
            + specular + emissionVal);

    FragColor = vec4(result, 1.0f);
}
