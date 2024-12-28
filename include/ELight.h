#ifndef _E_LIGHT_H

#define _E_LIGHT_H

enum LightType {
  UNDEFINED,
  AMBIENT,
  DIRECTIONAL,
  POINT,
  SPOTLIGHT,
};
#include "EObject3D.h"
#include <glm/ext/vector_float3.hpp>
class ELight : public EObject3D {
public:
  ELight();
  virtual ~ELight() = default;
  glm::vec3 ambient_color = glm::vec3(1.0f);
  glm::vec3 diffuse_color = glm::vec3(1.0f);
  glm::vec3 specular_color = glm::vec3(1.0f);
  glm::vec3 direction;
  float intensity;
  float linear;
  float quadratic;
  float inner_cutoff;
  float outer_cutoff;
  bool visualize = false;

  LightType type = UNDEFINED;
  void render(Shader &shader) override {
    // do nothing
  };
  void setInstanceUBO(unsigned int ubo, unsigned int count) override {};
  Type getType() override { return Type::Light; }
};

struct AmbientLight : public ELight {

  AmbientLight(glm::vec3 color, float intensity);
};
struct DirectionalLight : public ELight {
  DirectionalLight(glm::vec3 dif_col, glm::vec3 spec_color, glm::vec3 dir,
                   float intensity = 1.0f);
};

struct PointLight : public ELight {
  PointLight(glm::vec3 dif_col, glm::vec3 spec_color, glm::vec3 pos,
             float intensity = 1.0f, float linear = 0.14f,
             float quadratic = 0.07f);
};

struct SpotLight : public ELight {
  SpotLight(glm::vec3 dif_color, glm::vec3 spec_color, glm::vec3 pos,
            glm::vec3 dir, float intensity = 1.0f, float inner_cutoff = 0.423f,
            float outer_cutoff = 0.3);
};
#endif
