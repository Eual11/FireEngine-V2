#version 330

layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 tex_coord;

out vec2 TexCoord;
void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0f);
    TexCoord = tex_coord;
}
