#ifndef __MODEL_H
#define __MODEL_H
#include "../include/stb_image.h"
#include "Mesh.h"
#include "Shader.h"
#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <string>
#include <unordered_map>
#include <vector>
class Model {

public:
  Model(std::string path);
  glm::mat4 model_transform = glm::mat4(1.0f);
  void Draw(Shader &shader);

private:
  std::vector<Mesh> meshes;
  std::string dir;
  bool loadModel(std::string &path);
  void processNode(glm::mat4 m, aiNode *node, const aiScene *scene);
  Mesh processMesh(glm::mat4 &m, aiMesh *Mesh, const aiScene *scene);
  std::vector<Texture> loadMaterialTexture(aiMaterial *mat, aiTextureType type,
                                           std::string type_str);

  unsigned int textureFromFile(std::string path, std::string dir);
  std::unordered_map<std::string, Texture> loaded_textures;
};
#endif
