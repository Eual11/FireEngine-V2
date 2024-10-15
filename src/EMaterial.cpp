#include "../include/EMaterial.h"
#include <type_traits>
#include <variant>

Material::Material()
    : ambient_color(1.0f), diffuse_color(1.0f), specular_color(1.0f),
      shininess(0.0f) {}

PhongMaterial::PhongMaterial() : Material() {
  type = MaterialType::MaterialType_Phong;
}
BasicMaterial::BasicMaterial() : Material() {
  type = MaterialType::MaterialType_Basic;
}
BasicMaterial::BasicMaterial(glm::vec3 ambient_color, glm::vec3 diffuse_color,
                             glm::vec3 specular_color) {

  type = MaterialType::MaterialType_Basic;
  this->ambient_color = ambient_color;
  this->diffuse_color = diffuse_color;
  this->specular_color = specular_color;
  this->shininess = 0.0f;
}
void BasicMaterial::Apply(Shader &shader) {
  unsigned int numDiffuse = 1;
  unsigned int numSpecular = 1;
  bool diffuseBound = false;
  bool specularBound = false;

  shader.Use();

  shader.setVec3("material.diffuse_color", diffuse_color);

  for (size_t i = 0; i < textures.size(); i++) {
    ETexture tex = textures[i];
    std::string tex_name = "";
    glActiveTexture(GL_TEXTURE0 + i);
    if (tex.type == "texture_diffuse") {
      tex_name = tex.type + std::to_string(numDiffuse++);
      diffuseBound = true;
      shader.setInt("material." + tex_name, i);
      glBindTexture(GL_TEXTURE_2D, tex.ID);
    } else if (tex.type == "texture_specular") {
      tex_name = tex.type + std::to_string(numSpecular++);
      shader.setInt("material." + tex_name, i);
      specularBound = true;
      glBindTexture(GL_TEXTURE_2D, tex.ID);
    }
  }
  shader.setBool("material.diffuse_bound", diffuseBound);
  shader.setBool("material.specular_bound", specularBound);

  glActiveTexture(GL_TEXTURE0);
}

void PhongMaterial::Apply(Shader &shader) {
  unsigned int numDiffuse = 1;
  unsigned int numSpecular = 1;
  bool diffuseBound = false;
  bool specularBound = false;

  shader.Use();

  shader.setVec3("material.ambient_color", ambient_color);
  shader.setVec3("material.diffuse_color", diffuse_color);
  shader.setVec3("material.specular_color", specular_color);
  shader.setFloat("material.shininess", shininess);
  for (size_t i = 0; i < textures.size(); i++) {
    ETexture tex = textures[i];
    std::string tex_name = "";
    glActiveTexture(GL_TEXTURE0 + i);
    if (tex.type == "texture_diffuse") {
      tex_name = tex.type + std::to_string(numDiffuse++);
      diffuseBound = true;
      shader.setInt("material." + tex_name, i);
      glBindTexture(GL_TEXTURE_2D, tex.ID);
    } else if (tex.type == "texture_specular") {
      tex_name = tex.type + std::to_string(numSpecular++);
      shader.setInt("material." + tex_name, i);
      specularBound = true;
      glBindTexture(GL_TEXTURE_2D, tex.ID);
    }
  }
  shader.setBool("material.diffuse_bound", diffuseBound);
  shader.setBool("material.specular_bound", specularBound);

  glActiveTexture(GL_TEXTURE0);
}
void NormalMaterial::Apply(Shader &shader) { shader.Use(); }
NormalMaterial::NormalMaterial() { type = MaterialType_Normal; }

ShaderMaterial::ShaderMaterial(std::string vSrc, std::string fSrc) {
  vertexShaderSrc = vSrc;
  fragmentShaderSrc = fSrc;
  type = MaterialType_Shader;
}

ShaderMaterial::ShaderMaterial(std::string vSrc, std::string fSrc,
                               std::unordered_map<std::string, Uniform> unfs) {

  vertexShaderSrc = vSrc;
  fragmentShaderSrc = fSrc;
  uniforms = unfs;
  type = MaterialType_Shader;
}
void ShaderMaterial::Apply(Shader &shader) {
  shader.Use();
  for (auto &[name, value] : uniforms) {
    std::visit(
        [&shader, name](auto &&arg) {
          using T = std::decay_t<decltype(arg)>; // Removed parentheses here
          if constexpr (std::is_same_v<T, bool>) {
            shader.setBool(name, arg);
          } else if constexpr (std::is_same_v<T, int>) {
            shader.setInt(name, arg);
          } else if constexpr (std::is_same_v<T, float> ||
                               std::is_same_v<T, double>) {
            shader.setFloat(name, static_cast<float>(arg));
          } else if constexpr (std::is_same_v<T, glm::vec3>) {
            shader.setVec3(name, arg);
          } else if constexpr (std::is_same_v<T, glm::vec4>) {
            shader.setVec4(name, arg);
          } else if constexpr (std::is_same_v<T, glm::mat3>) {
            shader.setMat3(name, arg);
          } else if constexpr (std::is_same_v<T, glm::mat4>) {
            shader.setMat4(name, arg);
          }
        },
        value);
  }
}
