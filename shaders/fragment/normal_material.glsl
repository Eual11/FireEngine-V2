#version 330 core

in vec3 fragNormal;

out vec4 FragColor;

void main()
{
    FragColor = vec4(normalize(fragNormal) * 0.5 + 0.5, 1.0f);
}
