//skybox vertex shader
layout(location = 0) in vec3 aPos;

out vec3 skyboxTexCoord;
uniform mat4 uProjection;
uniform mat4 uView;
void main()
{
    skyboxTexCoord = aPos;
    mat4 view = mat4(mat3(uView));
    vec4 pos = uProjection * view * vec4(aPos, 1.0f);
    gl_Position = pos.xyww;
}
