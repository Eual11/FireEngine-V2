#ifndef _E_LIGHT_H

#define _E_LIGHT_H

enum LightType {
  UNDEFINED,
  AMBIENT,
  DIRECTIONAL,
  POINT,
  SPOTLIGHT,
};
#include <glm/ext/vector_float3.hpp>
struct ELight {
  ELight() = default;
  virtual ~ELight() = default;
  glm::vec3 ambient_color;
  glm::vec3 diffuse_color;
  glm::vec3 specular_color;
  LightType type = UNDEFINED;
};

struct AmbientLight : public ELight {
  float intensity;

  AmbientLight(glm::vec3 color, float intensity);
};
struct DirectionalLight : public ELight {
  DirectionalLight(glm::vec3 dif_col, glm::vec3 spec_color, glm::vec3 dir);
  glm::vec3 direction;
};
#endif
