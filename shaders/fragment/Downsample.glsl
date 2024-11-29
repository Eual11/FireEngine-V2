#version 330

in vec2 TexCoord;
out vec4 FragColor;

uniform vec2 uResolution;
uniform sampler2D screenTexture;

uniform float factor;
void main()
{
    //downsampling factor
    //aligning tex coord to a grid of a lower resolution
    vec2 baseCoord = floor(TexCoord * (uResolution / factor)) / (uResolution / factor);

    vec2 offset = vec2(factor) / uResolution;

    //applying a 2x2 box filter
    vec4 color = 0.25 * (
            texture(screenTexture, baseCoord) + //bottom left
                texture(screenTexture, baseCoord + vec2(offset.x, 0.0f)) + //bottom right
                texture(screenTexture, baseCoord + vec2(0.0f, offset.y)) + //top left
                texture(screenTexture, baseCoord + offset) //top right
            );

    FragColor = color;
}
