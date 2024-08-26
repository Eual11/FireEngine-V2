
// ELight.cpp
#include "../include/ELight.h"

// Default constructor and destructor are automatically provided
// by the compiler for simple cases.

// Constructor for AmbientLight
AmbientLight::AmbientLight(glm::vec3 color, float intensity) {
  type = AMBIENT;
  ambient_color = color;
  diffuse_color = color;
  specular_color = color;
  this->intensity = intensity;
}

// Constructor for DirectionalLight
DirectionalLight::DirectionalLight(glm::vec3 dif_col, glm::vec3 spec_color,
                                   glm::vec3 dir, float intensity) {
  type = DIRECTIONAL;
  ambient_color =
      dif_col; // Assuming the ambient color is the same as diffuse color
  diffuse_color = dif_col;
  specular_color = spec_color;
  direction = dir;
  this->intensity = intensity;
}
PointLight::PointLight(glm::vec3 dif_col, glm::vec3 spec_color, glm::vec3 pos,
                       float intensity, float linear, float quadratic) {
  type = POINT;
  ambient_color =
      dif_col; // Assuming the ambient color is the same as diffuse color
  diffuse_color = dif_col;
  specular_color = spec_color;
  position = pos;
  this->intensity = intensity;
  this->linear = linear;
  this->quadratic = quadratic;
}

SpotLight::SpotLight(glm::vec3 dif_color, glm::vec3 spec_color, glm::vec3 pos,
                     glm::vec3 dir, float intensity, float inner_cutoff,
                     float outer_cutoff) {
  type = SPOTLIGHT;
  diffuse_color = dif_color;
  specular_color = spec_color;
  position = pos;
  direction = dir;
  this->intensity = intensity;
  this->inner_cutoff = inner_cutoff;
  this->outer_cutoff = outer_cutoff;
}
