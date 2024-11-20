#version 330
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D screenTexture;
uniform float intensity;

void main()
{
    vec4 color = texture(screenTexture, TexCoord);

    float luminance = 0.21026 * color.r + 0.7152 * color.g + 0.0722 * color.b;
    vec3 greyScale = mix(color.rgb, vec3(luminance), intensity);
    FragColor = vec4(greyScale, 1.0f);
}
