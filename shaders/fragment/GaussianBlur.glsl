#version 330

//single pass gaussian blur for both x and y
//this can be optimized by separating the horizontal pass and vertical pass
//resulting in O(N) complixity instead of O(N^2), N is size of kernel

in vec2 TexCoord;
out vec4 FragColor;

uniform vec2 uResolution;
float PI = 3.1415926;
uniform sampler2D screenTexture;

void main()
{
    int radius = 10;
    int scale = 2;
    float sigma = 1;
    float sum = 0.0f;
    vec4 color = vec4(0.0f);

    for (int y = -radius; y <= radius; y++)
    {
        for (int x = -radius; x <= radius; x++)
        {
            float Q = 1.0f / (2.0f * PI * sigma * sigma);
            vec2 offset = vec2(float(x), float(y)) * scale / uResolution;
            vec4 col = texture(screenTexture, TexCoord + offset);
            float weight = Q * exp(-(x * x + y * y) / (2.0 * sigma * sigma));
            color += col * weight;
            sum += weight;
        }
    }

    color /= sum;
    FragColor = color;
}
