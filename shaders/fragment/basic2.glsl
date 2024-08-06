#version 330 core

in vec4 finalVertColor;
in vec2 finalTexCoord;

out vec4 FragColor;

uniform sampler2D texture2;
uniform sampler2D texture1;
//niform sampler2D texture2;
void main()
{
    FragColor = texture(texture2, finalTexCoord) * finalVertColor;
    FragColor = finalVertColor;
}
