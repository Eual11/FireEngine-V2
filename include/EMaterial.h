#ifndef _EMATERIAL_H
#define _EMATERIAL_H

#include "Shader.h"
#include <glm/ext/vector_float3.hpp>
#include <string>
#include <vector>
struct ETexture {
  unsigned int ID = 0;
  std::string type;
  std::string path;
};

enum MaterialType {

  MaterialType_Basic,   // flat
  MaterialType_Phong,   // basic phong shading
  MaterialType_Lambert, // lambert diffuse shading
  MaterialType_Normal   // used to visualize normals

};

class Material {

public:
  Material();
  glm::vec3 ambient_color;
  glm::vec3 diffuse_color;
  glm::vec3 specular_color;
  float shininess;
  std::vector<ETexture> textures;
  MaterialType type;

  virtual void Apply(Shader &shader) = 0;
  virtual ~Material() = default;
};

class PhongMaterial : public Material {

public:
  PhongMaterial();
  void Apply(Shader &shader) override;
  ~PhongMaterial() override = default;
};

#endif
