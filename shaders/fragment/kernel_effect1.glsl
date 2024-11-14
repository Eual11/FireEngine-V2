#version 330

in vec2 TexCoord;

out vec4 FragColor;
uniform sampler2D screenTexture

float offset = 1 / 300.0
void main ( )
{

vec2 offsets[9] =
    vec2[](
        vec2(-offset, offset), //topleft
        vec2(0.0f, offset), //top center
        vec2(offset, offset) //top right

        vec2(-offset, 0.0f), //center left
        vec2(0.0f, 0.0f), //center center
        vec2(offset, 0.0f), //top right

        vec2(-offset, -offset), //buttom right
        vec2(0.0f, -offset), //buttom center
        vec2(offset, -offset), //buttom right
    );

float kernel[9] = float[](
        -1, -1, -1,
        -1, 9, -1,
        -1, -1, -1
    );

vec3 sampledTex[9];

for ( int i = 0; i < 9 ; i ++ )
{
sampledTex[i] = vec3(texture(screenTexture, TexCoord.st+offsets[i]));
}

vec3 col = vec3(0.0f);
for ( int i = 0; i < 9 ; i ++ )
{
col += screenTexture[i] * kernel[i];
}

FragColor = vec4(col, 1.0f);

}
