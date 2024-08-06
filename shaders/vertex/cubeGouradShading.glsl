#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 normalVec;
//layout(location = 2) in vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 project;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewPos;

out vec4 finalVertColor;
out vec3 Normal;
out vec4 FragPos;
out vec4 LightPos;
out vec4 FragColor;

void main()
{
    gl_Position = project * view * model * vec4(aPos, 1.0f);
    FragPos = (model * vec4(aPos, 1.0f));
    LightPos = vec4(lightPos, 1.0f);
    Normal = mat3(transpose(inverse(model))) * normalVec;

    //ambient
    float ambient_strength = 0.1f;

    vec3 ambient = ambient_strength * lightColor;

    //diffuse
    vec3 lightDir = normalize(lightPos - FragPos.xyz);

    vec3 diffuse = max(dot(lightDir, Normal), 0.0f) * lightColor;

    //specular
    float specular_strength = 10.8;
    float mat_shin = 32;
    vec3 view_dir = normalize(viewPos - FragPos.xyz);

    vec3 reflVec = reflect(-lightDir, Normal);
    vec3 spec = pow(max(dot(view_dir, reflVec), 0.0f), mat_shin) * specular_strength * lightColor;
    vec3 result = (ambient + diffuse + spec) * objectColor;
    FragColor = vec4(result, 1.0f);
}
