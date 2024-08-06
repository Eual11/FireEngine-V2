#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 normalVec;
layout(location = 2) in vec2 tex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 project;
out vec3 Normal;
out vec4 FragPos;
out vec2 TexCoord;

void main()
{
    gl_Position = project * view * model * vec4(aPos, 1.0f);
    FragPos = ((model * vec4(aPos, 1.0f)));
    Normal = mat3(transpose(inverse(model))) * normalVec;
    TexCoord = tex;
}
