
#version 330 core

out vec4 FragColor; // Output color

uniform float time; // Time uniform variable

void main()
{
    // Create color using sine functions for smooth transitions
    float red = 0.5 + 0.5 * sin(time + 0.0); // Varies from 0 to 1
    float green = 0.5 + 0.5 * sin(time + 2.0); // Varies from 0 to 1
    float blue = 0.5 + 0.5 * sin(time + 4.0); // Varies from 0 to 1

    FragColor = vec4(red, green, blue, 1.0); // Set the output color with full opacity
}
