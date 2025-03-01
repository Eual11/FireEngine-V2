#version 330 core

layout(location = 0) in vec3 inPosition;
uniform mat4 uModel;
uniform mat4 lightSpaceTransform;
void main()
{
    gl_Position = lightSpaceTransform * uModel * vec4(inPosition, 1.0f);
}
