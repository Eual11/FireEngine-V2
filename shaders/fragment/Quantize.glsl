#version 330

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D screenTexture;

void main()
{
    int levels = 8;
    vec4 color = texture(screenTexture, TexCoord);
    color.r = floor((color.r * float(levels))) / levels;
    color.g = floor((color.g * float(levels))) / levels;
    color.b = floor((color.b * float(levels))) / levels;
    FragColor = color;
}
