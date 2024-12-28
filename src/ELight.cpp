
// ELight.cpp
#include "../include/ELight.h"

ELight::ELight() : EObject3D() {}
AmbientLight::AmbientLight(glm::vec3 color, float intensity) : ELight() {
  type = AMBIENT;
  ambient_color = color;
  diffuse_color = color;
  specular_color = color;
  this->intensity = intensity;
  name = "AmbientLight_" + std::to_string(ID);
}
DirectionalLight::DirectionalLight(glm::vec3 dif_col, glm::vec3 spec_color,
                                   glm::vec3 dir, float intensity)
    : ELight() {
  type = DIRECTIONAL;
  ambient_color =
      dif_col; // Assuming the ambient color is the same as diffuse color
  diffuse_color = dif_col;
  specular_color = spec_color;
  direction = dir;
  this->intensity = intensity;
  name = "DirectionalLight_" + std::to_string(ID);
}
PointLight::PointLight(glm::vec3 dif_col, glm::vec3 spec_color, glm::vec3 pos,
                       float intensity, float linear, float quadratic)
    : ELight() {
  type = POINT;
  ambient_color =
      dif_col; // Assuming the ambient color is the same as diffuse color
  diffuse_color = dif_col;
  specular_color = spec_color;
  Position = pos;
  this->intensity = intensity;
  this->linear = linear;
  this->quadratic = quadratic;

  name = "PointLight_" + std::to_string(ID);
}

SpotLight::SpotLight(glm::vec3 dif_color, glm::vec3 spec_color, glm::vec3 pos,
                     glm::vec3 dir, float intensity, float inner_cutoff,
                     float outer_cutoff)
    : ELight() {
  type = SPOTLIGHT;
  diffuse_color = dif_color;
  specular_color = spec_color;
  Position = pos;
  direction = dir;
  this->intensity = intensity;
  this->inner_cutoff = inner_cutoff;
  this->outer_cutoff = outer_cutoff;
  name = "SpotLight_" + std::to_string(ID);
}
