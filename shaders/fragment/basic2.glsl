#version 330 core

in vec2 fragTexCoord;

out vec4 FragColor;
uniform sampler2D tex;

float znear = 1;
float zfar = 10.0f;
float LinearizeDepth(float depth)
{
    float z = 2.0 * depth - 1.0f;

    return (2.0 * z * znear * zfar) / ((zfar + znear - z) * (zfar - znear));
}
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
    vec3 sampled_diffuse = vec3(texture(tex, fragTexCoord));

    float fogDensity = 0;
    vec3 fogColor = vec3(0.92, 0.913, 0.931);
    float depthVec = exp(-pow(LinearizeDepth(gl_FragCoord.z), fogDensity));
    FragColor = vec4(mix(sampled_diffuse, fogColor, fogDensity), 1.0f);
}
