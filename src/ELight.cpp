
// ELight.cpp
#include "../include/ELight.h"

// Default constructor and destructor are automatically provided
// by the compiler for simple cases.
// AmbientLight.cpp

// Constructor for AmbientLight
AmbientLight::AmbientLight(glm::vec3 color, float intensity) {
  ambient_color = color;
  diffuse_color = color;
  specular_color = color;
  type = AMBIENT;
  this->intensity = intensity;
}
// DirectionalLight.cpp

// Constructor for DirectionalLight
DirectionalLight::DirectionalLight(glm::vec3 dif_col, glm::vec3 spec_color,
                                   glm::vec3 dir, float intensity) {
  ambient_color =
      dif_col; // Assuming the ambient color is the same as diffuse color
  diffuse_color = dif_col;
  specular_color = spec_color;
  type = DIRECTIONAL;
  direction = dir;
  this->intensity = intensity;
}
