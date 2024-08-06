#version 330 core

in vec3 Normal;
in vec4 FragPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

out vec4 FragColor;
void main()
{

    //ambient light
    //

    //ambient light is the light that bounce off from the surrounding and lights and illuminates the object
    float ambient_strength = 0.1;

    vec3 ambient = lightColor * ambient_strength;

    //diffuse light
    //this simulates the light that randomly scatters around the surface of object due to surface irregularity
    //it depends on the angle of the surface

    vec3 light_dir = normalize(lightPos - FragPos.xyz);
    vec3 diffuse = max(dot(light_dir, Normal), 0.0f) * lightColor;

    //specular light
    //this simulates the highlights observed in shiny objects when the light's reflection angle aligns with the viewing angle creating a shiny spot

    float specular_strength = 2.6;

    vec3 viewDir = normalize(viewPos - FragPos.xyz);

    vec3 reflVec = reflect(-light_dir, Normal);

    float mShininess = 604;
    float spec = pow(max(dot(viewDir, reflVec), 0.0), mShininess);
    vec3 specular = spec * specular_strength * lightColor;
    vec3 result = (ambient + diffuse
            + specular) * objectColor;

    FragColor = vec4(result, 1.0f);
}
