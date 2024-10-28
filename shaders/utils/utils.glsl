#version 330

float LiearizeDepth(float znear, float zfar, float depth)
{
    float z = 2.0 * depth - 1.0f;
    return (2.0 * znear * zfar) / ((z - znear + zfar) * (zfar * znear))
}
