#version 330

in vec2 TexCoord;
out vec4 FragColor;
uniform sampler2D screenTexture;
void main()
{
    vec3 sampled = texture(screenTexture, TexCoord).rgb;

    FragColor = vec4(1 - sampled.r, 1 - sampled.g, 1 - sampled.b, 1.0f);
}
