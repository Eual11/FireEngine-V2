#version 330

in vec2 TexCoord;

out vec4 FragColor;

uniform vec2 uResolution;

#define PI 3.1415926

uniform sampler2D screenTexture;

void main()
{
    float sigma = 4.0f;
    int radius = 7;
    int scale = 2;

    float sum = 0.0f;

    vec4 color = vec4(0.0f);

    for (int i = -radius; i <= radius; i++)
    {
        float Q = 1.0f / (sqrt(2 * PI) * sigma);
        float weight = Q * exp(-(i * i) / (2 * sigma * sigma));
        vec2 offset = vec2(float(i), float(0)) * scale / uResolution;
        vec4 col = texture(screenTexture, TexCoord + offset);
        color += col * weight;
        sum += weight;
    }

    color /= sum;

    FragColor = color;
}
