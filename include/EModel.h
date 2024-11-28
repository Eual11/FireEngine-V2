#ifndef __EMODELN_H
#define __EMODELN_H
#include "../include/stb_image.h"
#include "EGroup.h"
#include "EMaterial.h"
#include "EMesh.h"
#include "EObject3D.h"
#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/types.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <memory>
class EModel : public EObject3D {

public:
  std::shared_ptr<EGroup> rootNode = nullptr;

public:
  EModel(std::string);
  EModel() = default;
  void loadModel(std::string path);
  void render(Shader &shader) override;
  void enableInstanced(const std::vector<glm::mat4> &matrices);
  Type getType() override;
  std::shared_ptr<Material> getMaterial() const { return material; }
  void setMaterial(std::shared_ptr<Material> mat) { this->material = mat; }
  void setInstanceUBO(unsigned int ubo, unsigned int count) override {};

private:
  std::shared_ptr<Material> material = nullptr;
  bool instanced = false;
};

#endif
