//skybox fragment shader
#version 330 core
in vec3 skyboxTexCoord;
out vec4 FragColor;
uniform samplerCube skybox;
void main()
{
    FragColor = texture(skybox, skyboxTexCoord);
}
