#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 vertColor;
layout(location = 2) in vec2 TexCoord;

out vec4 finalVertColor;
out vec2 finalTexCoord;
uniform mat4 view;
uniform mat4 project;
uniform mat4 model;
void main()
{
    gl_Position = project * view * model * vec4(aPos, 1.0f);
    finalVertColor = vec4(vertColor, 1.0f);
    finalTexCoord = TexCoord;
}
