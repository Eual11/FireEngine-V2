#version 330

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inNormal;
layout(location = 2) in vec2 inTexCoord;

uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    gl_Position = uProjection * uView * vec4(inPosition, 1.0f);
}
