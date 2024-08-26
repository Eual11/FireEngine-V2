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
  float intensity;
  LightType type = UNDEFINED;
};

struct AmbientLight : public ELight {
  float intensity;

  AmbientLight(glm::vec3 color, float intensity);
};
struct DirectionalLight : public ELight {
  DirectionalLight(glm::vec3 dif_col, glm::vec3 spec_color, glm::vec3 dir,
                   float intensity = 1.0f);
  glm::vec3 direction;
};

struct PointLight : public ELight {
  PointLight(glm::vec3 dif_col, glm::vec3 spec_color, glm::vec3 pos,
             float intensity = 1.0f, float linear = 0.7f,
             float quadratic = 1.8f);
  glm::vec3 position;
  float linear;
  float quadratic;
};
#endif
