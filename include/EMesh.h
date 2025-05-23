#ifndef _E_MESH_H
#define _E_MESH_H

#include "EBufferGeometry.h"
#include "EMaterial.h"
#include "EObject3D.h"
#include "Shader.h"
#include <glad/glad.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

class EMesh : public EObject3D {

public:
  EMesh(std::vector<EVertex>, std::vector<ETexture>, std::vector<unsigned int>);

  EMesh(std::shared_ptr<EBufferGeometry> geo, std::shared_ptr<Material> mat);
  std::shared_ptr<EBufferGeometry> geometry = nullptr;

  ~EMesh();
  void render(Shader &) override;
  Type getType() override { return Type::Mesh; }
  void setMaterial(std::shared_ptr<Material> mat) { material = mat; };
  std::shared_ptr<Material> getMaterial() const { return material; };
  bool isInstanced() const { return instanced; }
  void enableInstanced(const std::vector<glm::mat4> &matrices) {

    instanced = true;
    instanceMatricies = std::vector<glm::mat4>(matrices);

    // HACK: this is for testing purpose only
    // for mesh

    unsigned int instabuffer;
    glGenBuffers(1, &instabuffer);
    glBindBuffer(GL_ARRAY_BUFFER, instabuffer);

    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * instanceMatricies.size(),
                 &instanceMatricies[0], GL_STATIC_DRAW);

    setInstanceUBO(instabuffer, instanceMatricies.size());
  }
  void setInstanceUBO(unsigned int ubo, unsigned int count) override {

    instanced = true;
    this->instaceCount = count;
    glBindBuffer(GL_ARRAY_BUFFER, ubo);
    geometry->bind();

    size_t v4s = sizeof(glm::vec4);
    const int BASE_ATTRIBUTE_INDEX = 4;

    for (int i = 0; i < 4; ++i) {
      int attribIndex = BASE_ATTRIBUTE_INDEX + i;
      glEnableVertexAttribArray(attribIndex);
      glVertexAttribPointer(attribIndex, 4, GL_FLOAT, GL_FALSE, 4 * v4s,
                            (void *)(i * v4s));
      glVertexAttribDivisor(attribIndex, 1);
    }
    geometry->unbind();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

private:
  std::vector<ETexture> textures;
  std::shared_ptr<Material> material = nullptr;

  unsigned int VAO, VBO, EBO;

  bool instanced = false;
  unsigned int instaceCount = 0;
  std::vector<glm::mat4> instanceMatricies;
};

#endif
