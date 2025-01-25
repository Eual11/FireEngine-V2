#include "../include/EMaterial.h"
#include <memory>
#include <string>
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
  unsigned int numNormal = 1;
  bool diffuseBound = false;
  bool specularBound = false;
  bool normalBound = false;

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
    } else if (tex.type == "texture_normal") {
      glBindTexture(GL_TEXTURE_2D, tex.ID);
      tex_name = tex.type + std::to_string(numNormal++);
      shader.setInt("material." + tex_name, i);
      normalBound = true;
    }
  }
  shader.setBool("material.diffuse_bound", diffuseBound);
  shader.setBool("material.specular_bound", specularBound);
  shader.setBool("material.normal_bound", normalBound);

  glActiveTexture(GL_TEXTURE0);
}

void PhongMaterial::Apply(Shader &shader) {
  unsigned int numDiffuse = 1;
  unsigned int numSpecular = 1;
  unsigned int numNormal = 1;

  // flags to see if a specfic texture type is bound or not
  bool diffuseBound = false;
  bool specularBound = false;
  bool normalBound = false;

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
    } else if (tex.type == "texture_normal") {
      glBindTexture(GL_TEXTURE_2D, tex.ID);
      tex_name = tex.type + std::to_string(numNormal++);
      shader.setInt("material." + tex_name, i);
      normalBound = true;
    }
  }
  shader.setBool("material.diffuse_bound", diffuseBound);
  shader.setBool("material.specular_bound", specularBound);
  shader.setBool("material.normal_bound", normalBound);

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
  unsigned int texUnitIndex = 0;
  for (auto &[name, value] : uniforms) {
    std::visit(
        [&shader, name, &texUnitIndex](auto &&arg) {
          using T = std::decay_t<decltype(arg)>;
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
          } else if constexpr (std::is_same_v<T, std::shared_ptr<ETexture>>) {
            glActiveTexture(GL_TEXTURE0 + texUnitIndex);
            shader.setInt(name, texUnitIndex++);
            glBindTexture(GL_TEXTURE_2D, arg->ID);
          }
        },
        value);
  }
  glActiveTexture(GL_TEXTURE0);
}
PBRMaterial::PBRMaterial() : Material() { type = MaterialType_PBR; }
void PBRMaterial::Apply(Shader &shader) {
  unsigned int numAlbedo = 1;
  unsigned int numMetalic = 1;
  unsigned int numRoughness = 1;
  unsigned int numAO = 1;
  unsigned int numNormal = 1;

  // flags to see if a specfic texture type is bound or not
  bool albedoBound = false;
  bool metalicBound = false;
  bool roughnessBound = false;
  bool AOBound = false;
  bool normalBound = false;

  shader.Use();

  shader.setVec3("material.albedo", albedo);
  shader.setFloat("material.roughness", roughness);
  shader.setFloat("material.metalic", metalic);

  for (size_t i = 0; i < textures.size(); i++) {
    ETexture tex = textures[i];
    std::string tex_name = "";
    glActiveTexture(GL_TEXTURE0 + i);
    if (tex.type == "texture_albedo") {
      tex_name = tex.type + std::to_string(numAlbedo++);
      albedoBound = true;
      shader.setInt("material." + tex_name, i);
      glBindTexture(GL_TEXTURE_2D, tex.ID);
    } else if (tex.type == "texture_metalic") {
      tex_name = tex.type + std::to_string(numMetalic++);
      shader.setInt("material." + tex_name, i);
      metalicBound = true;
      glBindTexture(GL_TEXTURE_2D, tex.ID);
    } else if (tex.type == "texture_normal") {
      glBindTexture(GL_TEXTURE_2D, tex.ID);
      tex_name = tex.type + std::to_string(numNormal++);
      shader.setInt("material." + tex_name, i);
      normalBound = true;
    } else if (tex.type == "texture_roughness") {
      glBindTexture(GL_TEXTURE_2D, tex.ID);
      tex_name = tex.type + std::to_string(numRoughness++);
      shader.setInt("material." + tex_name, i);
      roughnessBound = true;
    } else if (tex.type == "texture_ao") {
      glBindTexture(GL_TEXTURE_2D, tex.ID);
      tex_name = tex.type + std::to_string(numAO++);
      shader.setInt(tex_name, i);
    }
  }
  shader.setBool("material.abledo_bound", albedoBound);
  shader.setBool("material.metalic_bound", metalicBound);
  shader.setBool("material.normal_bound", normalBound);
  shader.setBool("material.roughness_bound", roughnessBound);
  shader.setBool("material.ao_bound", AOBound);

  glActiveTexture(GL_TEXTURE0);
}
