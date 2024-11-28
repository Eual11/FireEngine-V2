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
  ELight() = default;
  virtual ~ELight() = default;
  glm::vec3 ambient_color;
  glm::vec3 diffuse_color;
  glm::vec3 specular_color;
  float intensity;
  LightType type = UNDEFINED;
  void render(Shader &shader) override {
    // do nothing
  };
  void setInstanceUBO(unsigned int ubo, unsigned int count) override {};
  Type getType() override { return Type::Light; }
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
             float intensity = 1.0f, float linear = 0.14f,
             float quadratic = 0.07f);
  glm::vec3 position;
  float linear;
  float quadratic;
  // Set the position of the PointLight
  void setPosition(float x, float y, float z) { position = glm::vec3(x, y, z); }

  // Overload for setting position using glm::vec3
  void setPosition(const glm::vec3 &newPos) { position = newPos; }
  // 0.045	0.0075
};

struct SpotLight : public ELight {
  glm::vec3 position;
  glm::vec3 direction;
  float inner_cutoff;
  float outer_cutoff;
  float intensity;

  SpotLight(glm::vec3 dif_color, glm::vec3 spec_color, glm::vec3 pos,
            glm::vec3 dir, float intensity = 1.0f, float inner_cutoff = 0.423f,
            float outer_cutoff = 0.3);
  // Set the position of the SpotLight
  void setPosition(float x, float y, float z) { position = glm::vec3(x, y, z); }

  // Overload for setting position using glm::vec3
  void setPosition(const glm::vec3 &newPos) { position = newPos; }
};
#endif
