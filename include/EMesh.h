#ifndef _E_MESH_H
#define _E_MESH_H

#include "EBufferGeometry.h"
#include "EMaterial.h"
#include "EObject3D.h"
#include "Shader.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

class EMesh : public EObject3D, std::enable_shared_from_this<EMesh> {

public:
  EMesh(std::vector<EVertex>, std::vector<ETexture>, std::vector<unsigned int>);

  EMesh(std::shared_ptr<EBufferGeometry> geo, std::shared_ptr<Material> mat);
  std::shared_ptr<EBufferGeometry> geometry = nullptr;

  ~EMesh();
  void render(Shader &) override;
  Type getType() override { return Type::Mesh; }

private:
  std::vector<ETexture> textures;
  std::shared_ptr<Material> material;

  unsigned int VAO, VBO, EBO;
};

#endif
