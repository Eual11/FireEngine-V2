#ifndef _EMATERIAL_H
#define _EMATERIAL_H

#include "Shader.h"
#include <glm/ext/vector_float3.hpp>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

using Uniform = std::variant<bool, int, float, double, glm::vec4, glm::vec3,
                             glm::mat4, glm::mat3>;
using UniformMap = std::unordered_map<std::string, Uniform>;
struct ETexture {
  unsigned int ID = 0;
  std::string type;
  std::string path;
};

enum MaterialType {

  MaterialType_Basic,   // flat
  MaterialType_Phong,   // basic phong shading
  MaterialType_Lambert, // lambert diffuse shading
  MaterialType_Normal,  // used to visualize normals
  MaterialType_Shader

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

class BasicMaterial : public Material {

public:
  BasicMaterial();
  BasicMaterial(glm::vec3 ambient_color, glm::vec3 diffuse_color,
                glm::vec3 specular_color);

  void Apply(Shader &shader) override;
  ~BasicMaterial() override = default;
};

class PhongMaterial : public Material {

public:
  PhongMaterial();

  void Apply(Shader &shader) override;
  ~PhongMaterial() override = default;
};
class NormalMaterial : public Material {
public:
  NormalMaterial();
  void Apply(Shader &shader) override;
  ~NormalMaterial() override = default;
};

// Finally
class ShaderMaterial : public Material {

public:
  ShaderMaterial() { type = MaterialType_Shader; };
  ShaderMaterial(std::string vSrc, std::string fSrc);
  ShaderMaterial(std::string vSrc, std::string fSrc,
                 std::unordered_map<std::string, Uniform>);
  void Apply(Shader &shader) override;
  ~ShaderMaterial() override = default;
  std::string vertexShaderSrc = "";
  std::string fragmentShaderSrc = "";
  std::unordered_map<std::string, Uniform> uniforms;
};
#endif
