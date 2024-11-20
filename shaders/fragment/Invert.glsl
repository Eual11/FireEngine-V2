#version 330
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D screenTexture;

void main()
{
    vec4 color = texture(screenTexture, TexCoord);
    vec3 inverted = vec3(1 - color.r, 1 - color.g, 1 - color.b);
    FragColor = vec4(inverted, 1.0f);
}
