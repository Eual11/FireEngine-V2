// =====================================================
//  Shader: Blit a texture, this shader is used on the final pass of post processing effects
//  to render the final result
// Purpose: Blit/Render the final result of all post processing pipeline chain to the screen//
// Inputs:
//  - TexCoord (vec2): Texture coordinates from the vertex shader.
//
// Outputs:
//  - FragColor (vec4): final color output
// //
// Uniforms:
//  - screenTexture (sampler2D): screen texture to sample
// =====================================================

#version 330

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D screenTexture;

void main()
{
    vec3 color = texture(screenTexture, TexCoord).rgb;

    FragColor = vec4(color, 1.0f);
}
