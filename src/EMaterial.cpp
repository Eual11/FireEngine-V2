#include "../include/EMaterial.h"

Material::Material()
    : ambient_color(1.0f), diffuse_color(1.0f), specular_color(1.0f),
      shininess(0.0f) {}

PhongMaterial::PhongMaterial() : Material() {
  type = MaterialType::MaterialType_Phong;
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
  shader.setBool("material.diffse_bound", diffuseBound);
  shader.setBool("material.specular_bound", specularBound);

  glActiveTexture(GL_TEXTURE0);
}
