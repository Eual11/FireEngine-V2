#ifndef _EMODEL_H
#define _EMODEL_H

#include "../include/stb_image.h"
#include "EMesh.h"
#include "EObject3D.h"
#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/types.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <unordered_map>

class EModel : public EObject3D {

private:
  std::vector<EMesh> meshes;
  std::string root_path;

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
  void render(Shader &shader);
};

#endif
