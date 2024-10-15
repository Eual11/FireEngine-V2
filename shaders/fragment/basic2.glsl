#version 330 core

in vec4 finalVertColor;
in vec2 finalTexCoord;

in vec2 fragTexCoord;
out vec4 FragColor;
struct Material
{
    //we might use this...
    vec3 ambient_color;
    vec3 diffuse_color;
    vec3 specular_color;
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    bool diffuse_bound;
    bool specular_bound;
    float shininess;
};
uniform Material material;
//niform sampler2D texture2;
void main()
{
    vec3 sampled_diffuse = material.diffuse_color;
    if (material.diffuse_bound)
        sampled_diffuse = vec3(texture(material.texture_specular1, fragTexCoord));

    FragColor = vec4(sampled_diffuse, 1.0f);
}
