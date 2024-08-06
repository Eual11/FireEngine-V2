#version 330 core

in vec3 Normal;
in vec4 FragPos;
in vec4 FragColor;
in vec4 Lightpos;

void main()
{
    gl_FragColor = FragColor;
}
