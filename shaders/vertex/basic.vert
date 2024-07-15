#version 330 core
layout (location = 0) in vec3 aPos;
layout (location=1) in vec3 our_color;
out vec4 vertColor;
void main()
{
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
vertColor = vec4(our_color, 1.0f);
}

