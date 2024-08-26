# Vertex Shader Documentation

## Overview 
- The vertex shader is a program that accpets input from the vertex buffer tells what is their position in clip-space which is easier for the fragment shader to process and render the graphics we want

### Naming Conventions 
- the naming convention followed is camelCasing
- all input from layout location must be prefixed with `in` Example: `inPosition`
- output data should be prefixed with lower case shortened name of the next stage of the pipeline, if we are passing to fragment shader, `fragPosition`
- uniform should be prefixed with `u` 
example: `uDeltaTime`

### Inputs 
1. `inPosition`: `vec3` - Vertex position in object space
2. `inNormal`: `vec3`- Normal associated with the vertex
3. `inTexCoord`: `vec2` Texture Coordinate

### Uniform

1. `uModel`: Transforms Object to world space 
2. `uView`: Transforms Object space to camera/view space
3. `uProjection`: Transforms camera/view space to clip space
4. `uViewPos`: Camera's position in world space
5. `uDeltaTime`: Time elapsed in seconds since the last frame used for frame independent physics
6. `uTime`: global time, time elapsed in seconds since the start of the program

### Output
1. `fragPosition`: `vec3` vertex position in world space 
2. `fragNormal`: `vec3` vertex normal transformed by the transpose inverse of model transform
3. `fragTexCoord`:`vec2` texture Coordinate




