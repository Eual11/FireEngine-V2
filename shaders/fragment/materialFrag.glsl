#version 330 core

struct Material
{
    //this represents the different color/reflection and absorbition of the material under different condition
    vec3 ambient; //the surface's color under ambient light
    vec3 diffuse; //the surface's color under diffuse light
    vec3 specular; //the surface's color under specular light
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
uniform Light light;
uniform vec3 viewPos;
uniform Material material;

out vec4 FragColor;
void main()
{

    //ambient light
    //

    //ambient light is the light that bounce off from the surrounding and lights and illuminates the object

    vec3 ambient = light.ambient * material.ambient;

    //diffuse light
    //this simulates the light that randomly scatters around the surface of object due to surface irregularity
    //it depends on the angle of the surface

    vec3 light_dir = normalize(light.position - FragPos.xyz);
    float diff = max(dot(light_dir, Normal), 0);
    vec3 diffuse = light.diffuse * (material.diffuse * diff);
    //specular light
    //this simulates the highlights observed in shiny objects when the light's reflection angle aligns with the viewing angle creating a shiny spot

    float specular_strength = 2.6;

    vec3 viewDir = normalize(viewPos - FragPos.xyz);

    vec3 reflVec = reflect(-light_dir, Normal);

    float spec = pow(max(dot(viewDir, reflVec), 0.0), material.shineness);
    vec3 specular = (spec * material.specular) * light.specular;
    vec3 result = (ambient + diffuse
            + specular);

    FragColor = vec4(result, 1.0f);
}
