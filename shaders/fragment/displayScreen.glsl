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

vec3 reinhard_luminance(vec3 v);
vec3 reinhard_simple(vec3 v);
// Controls for final post processing pass
// for now only tone mapping and gama correction
uniform bool enableToneMapping;
uniform bool enableGammaCorrection;

float luminance(vec3 v);

vec3 change_luminance(vec3 c_in, float l_out)
{
    float l_in = luminance(c_in);
    return c_in * (l_out / l_in);
}vec3 reinhard_extended_luminance(vec3 v, float max_white_l)
{
    float l_old = luminance(v);
    float numerator = l_old * (1.0f + (l_old / (max_white_l * max_white_l)));
    float l_new = numerator / (1.0f + l_old);
    return change_luminance(v, l_new);
}void main()
{
    vec3 color = texture(screenTexture, TexCoord).rgb;

    // Reinhard ToneMapping
    if (enableToneMapping)
    {
        color = reinhard_simple(color);
    }
    const float gamma = 2.2;
    // Applying gamma correction

    if (enableGammaCorrection)
    {
        color = pow(color, vec3(1.0 / gamma));
    }

    FragColor = vec4(color, 1.0f);
}

float luminance(vec3 v)
{
    return dot(v, vec3(0.2126f, 0.7152f, 0.0722f));
}

vec3 reinhard_simple(vec3 v)
{
    return v / (v + vec3(1.0f));
}

vec3 reinhard_luminance(vec3 v)
{
    float l = luminance(v);

    return v / (1.0 + l);
}
