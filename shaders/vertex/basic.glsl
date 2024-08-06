#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 vertColor;
layout(location = 2) in vec2 tex_cord;

out vec4 finalVertColor;
out vec2 TexCoord;
void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    finalVertColor = vec4(vertColor, 1.0f);
    TexCoord = tex_cord;
}
