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
  void setMaterial(std::shared_ptr<Material> mat) { material = mat; };
  std::shared_ptr<Material> getMaterial() const { return material; };
  bool isInstanced() const { return instanced; }
  void enableInstanced(const std::vector<glm::mat4> &matrices) {

    instanced = true;
    instanceMatricies = matrices;

    geometry->bind();

    unsigned int instanceBuffer;

    // HACK: this is for testing purpose only
    // for mesh
    glGenBuffers(1, &instanceBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * instanceMatricies.size(),
                 instanceMatricies.data(), GL_STATIC_DRAW);
    size_t v4s = sizeof(glm::vec4);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * v4s, (void *)(0));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * v4s, (void *)(v4s * 1));

    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * v4s, (void *)(v4s * 2));

    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * v4s, (void *)(v4s * 3));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    geometry->unbind();

    GLenum err;

    while ((err = glGetError()) != GL_NO_ERROR) {
      std::cerr << "OpenGL Error: " << std::hex << err << std::endl;
    }
  }

private:
  std::vector<ETexture> textures;
  std::shared_ptr<Material> material = nullptr;

  unsigned int VAO, VBO, EBO;

  bool instanced = false;
  std::vector<glm::mat4> instanceMatricies;
};

#endif
