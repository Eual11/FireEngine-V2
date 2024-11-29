#version 330 core
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in mat4 instanceMatrix;
uniform mat4 uModel;
uniform mat4 uView;

uniform mat4 uProjection;
uniform bool uInstanced;
uniform float uTime;
uniform float radius;

out vec4 fragPosition;
out vec3 fragNormal;
out vec2 fragTexCoord;

void main()
{
    float velocity = 0.4;
    float angularVelocity = velocity / radius;
    float theta = angularVelocity * uTime;
    vec4 pos = vec4(inPosition, 1.0f);
    vec4 dist = vec4(sin(theta) * radius, 0, cos(theta) * radius, 0.0f);
    mat4 transformMatrix = uModel;
    pos += dist;
    if (uInstanced)
    {
        transformMatrix = instanceMatrix;
        gl_Position = uProjection * uView * transformMatrix * pos;
    }
    else {
        gl_Position = uProjection * uView * uModel * vec4(inPosition, 1.0f);
    }

    fragPosition = ((uModel * vec4(inPosition, 1.0f)));
    fragNormal = normalize(mat3(transpose(inverse(uModel))) * inNormal);

    fragTexCoord = inTexCoord;
}
