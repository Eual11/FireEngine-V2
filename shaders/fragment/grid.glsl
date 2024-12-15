#version 330

out vec4 FragColor;
in vec3 near_point;
in vec3 far_point;

uniform float uZFar;
uniform float uZnear;
uniform mat4 uView;
uniform mat4 uProjection;

vec4 grid(vec3 frag_coord, float scale)
{
    //extracting x and z coordintes and scaling them
    //higher scale means denser lines
    vec2 coord = frag_coord.xz * scale;
    vec2 derivative = fwidth(coord);

    //fract(coord-0.5) wraps frag_coord to a unit square centred at 0.5
    //abs(frac(...)-0.5) is the distance between the nearest grid line
    //division by derivative is normalization
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
    //epresent the proximity of the fragment to the grid lines along the x and z axes, respectively.
    //minimum distance
    //epresent the proximity of the fragment to the grid lines along the x and z axes, respectively.
    float line = min(grid.x, grid.y);

    float minimumX = min(derivative.x, 1.0f);
    float minimumZ = min(derivative.y, 1.0f);

    vec4 color = vec4(0.2, 0.2, 0.2, 1.0f - min(line, 1.0f));
    if (frag_coord.x > -0.5 * minimumX && frag_coord.x < 0.5 * minimumX)
        color.x = 1.0f;
    if (frag_coord.z > -0.5 * minimumZ && frag_coord.z < 0.5 * minimumZ)
        color.z = 1.0f;

    return color;
}
float compute_depth(vec3 pos)
{
    vec4 proj = uProjection * uView * vec4(pos, 1.0f);

    return proj.z / proj.w;
}
float compute_linear_depth(vec3 pos)
{
    vec4 clipspace = uProjection * uView * vec4(pos, 1.0f);

    //range from [0,1]
    float clip_depth = clipspace.z / clipspace.w;
    //putting to range [-1,1]
    clip_depth = clip_depth * 2.0f - 1.0f;

    float linear_depth =
        (2.0f * uZFar * uZnear) / (uZnear + uZFar - clip_depth * (uZFar - uZnear));

    //normalization
    return linear_depth / uZFar;
}
void main()
{
    float t = -near_point.y / (far_point.y - near_point.y);
    vec3 frag_coord = near_point + t * (far_point - near_point);

    float linearDepth = compute_linear_depth(frag_coord);
    float fading = max(0, (0.5 - linearDepth));
    gl_FragDepth = compute_depth(frag_coord);
    FragColor = grid(frag_coord, 1.0f) * float(t > 0);
    FragColor.a *= fading;
}
