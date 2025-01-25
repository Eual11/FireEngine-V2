#ifndef __EMODEL_LOADER__
#define __EMODEL_LOADER__
#include "../include/stb_image.h"
#include "EGroup.h"
#include "EMesh.h"
#include "EModel.h"
#include "EObject3D.h"
#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/types.h>
#include <filesystem>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <memory>

class EModelLoader {
private:
  std::string root_path;

  std::unordered_map<std::string, unsigned int> loaded_texture;

  std::shared_ptr<EObject3D> ProcessNode(glm::mat4 m, aiNode *node,
                                         const aiScene *scene);
  std::shared_ptr<EMesh> ProcessMesh(glm::mat4 &m, aiMesh *mesh,
                                     const aiScene *scene);

  // loading material
  std::vector<ETexture> loadMaterialTexture(aiMaterial *mat, aiTextureType type,
                                            std::string name);
  unsigned int TextureFromFile(std::string path);

  std::shared_ptr<PhongMaterial> fetchPhongMaterial(aiMaterial *mat);
  std::shared_ptr<PBRMaterial> fetchPBRMaterial(aiMaterial *mat);

public:
  EModelLoader() = default;
  std::shared_ptr<EModel>
  loadModel(std::string path, std::shared_ptr<Material> material = nullptr);
};

#endif
