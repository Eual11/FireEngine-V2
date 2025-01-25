#version 330 core
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inTangent;
layout(location = 4) in mat4 instanceMatrix;
uniform mat4 uModel;
uniform mat4 uView;

uniform mat4 uProjection;
uniform bool uInstanced;
uniform float uTime;
uniform float radius;

out vec4 fragPosition;
out vec3 fragNormal;
out vec2 fragTexCoord;
out mat3 TBN;

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
        transformMatrix = instanceMatrix * uModel;
        gl_Position = uProjection * uView * transformMatrix * pos;
    }
    else {
        gl_Position = uProjection * uView * uModel * vec4(inPosition, 1.0f);
    }

    vec3 T = normalize(vec3(uModel * vec4(inTangent, 0.0f)));
    vec3 N = normalize(vec3(uModel * vec4(inNormal, 0.0f)));

    //renormalizing Tangent using Gram-Schmit Process
    T = normalize(T - (dot(T, N) * N));

    vec3 B = cross(N, T);

    TBN = mat3(T, B, N);
    fragPosition = ((uModel * vec4(inPosition, 1.0f)));
    fragNormal = normalize(mat3(transpose(inverse(uModel))) * inNormal);

    fragTexCoord = inTexCoord;
}
