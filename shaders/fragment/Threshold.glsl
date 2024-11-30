#version 330

uniform sampler2D screenTexture;
in vec2 TexCoord;
out vec4 FragColor;
void main()
{
    float threshold = 0.1;
    vec3 color = texture(screenTexture, TexCoord).rgb;
    //luminance=R⋅0.2126+G⋅0.7152+B⋅0.0722
    float luminance = dot(color, vec3(0.2126, 0.7152, 0.0722));

    if (luminance >= threshold)
    {
        FragColor = vec4(color, 1.0f);
    }
    else {
        FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
}
