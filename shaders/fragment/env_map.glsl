
out vec4 FragColor;

in vec4 fragPosition;
in vec3 fragNormal;
uniform vec3 uViewPos;
uniform samplerCube envMap;
void main()
{
    float ratio = 1 / 1.52;
    vec3 I = normalize(vec3(fragPosition) - uViewPos);
    //vec3 R = reflect(I, normalize(fragNormal));
    vec3 R = refract(I, normalize(fragNormal), ratio);
    //FragColor = vec4(1.0f);
    FragColor = texture(envMap, R);
}
