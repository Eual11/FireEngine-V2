#version 330

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inNormal;
layout(location = 2) in vec2 inTexCoord;

out vec3 near_point;
out vec3 far_point;

uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    vec3 pos = inPosition;
    mat4 inv_proj = inverse(uProjection);
    mat4 inv_view = inverse(uView);

    vec4 clipspace = vec4(inPosition.xy, 0.0f, 1.0f);

    vec4 v;
    v = (inv_view * inv_proj * clipspace);
    v /= v.w;

    near_point = v.xyz;

    clipspace.z = 1.0f;
    v = (inv_view * inv_proj * clipspace);
    v /= v.w;
    far_point = v.xyz;
    gl_Position = vec4(inPosition, 1.0f);
}
