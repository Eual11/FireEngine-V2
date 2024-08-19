#ifndef _EMODEL_H
#define _EMODEL_H

#include "../include/stb_image.h"
#include "EMesh.h"
#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/types.h>
#include <filesystem>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <unordered_map>

class EModel {

private:
  std::vector<EMesh> meshes;
  std::string root_path;
  glm::vec3 position = {0.0f, 0.0f, 0.0f};
  glm::vec3 rotation = {0.0f, 0.0f, 0.0f};
  glm::vec3 scale = {1.0f, 1.0f, 1.0f};
  glm::mat4 model_transform;

  std::unordered_map<std::string, unsigned int> loaded_texture;

  void ProcessNode(glm::mat4 m, aiNode *node, const aiScene *scene);
  EMesh ProcessMesh(glm::mat4 &m, aiMesh *mesh, const aiScene *scene);

  // loading material
  std::vector<ETexture> loadMaterialTexture(aiMaterial *mat, aiTextureType type,
                                            std::string name);
  unsigned int TextureFromFile(std::string path);

public:
  EModel(std::string);
  void loadModel(std::string path);
  void Draw(Shader &shader);
  glm::mat4 getModelTransform();
  glm::mat4 getModelTransformStatic();
  void setPosition(float x, float y, float z);
  glm::vec3 getPosition();
  void setRotation(float fYaw, float fPitch, float fRoll);
  glm::vec3 getRotation();
  void setScale(float x, float y, float z);
  glm::vec3 getScale();
};

#endif
