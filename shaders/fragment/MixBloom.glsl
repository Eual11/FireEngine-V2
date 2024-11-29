#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D screenTexture;
uniform sampler2D bloomTexture;

void main() {
    vec4 sceneColor = texture(screenTexture, TexCoord);
    vec4 bloomColor = texture(bloomTexture, TexCoord);

    FragColor = sceneColor + bloomColor; // Additive blending
}
