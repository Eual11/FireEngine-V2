#include "../include/EMesh.h"
EMesh::EMesh(std::vector<EVertex> verts, std::vector<ETexture> texs,
             std::vector<unsigned int> indx)
    : EObject3D() {

  this->geometry =
      std::make_shared<EBufferGeometry>(EBufferGeometry(verts, indx));

  this->textures = texs;
}
EMesh::EMesh(std::shared_ptr<EBufferGeometry> geo,
             std::shared_ptr<Material> mat)
    : EObject3D() {
  this->geometry = geo;
  if (!mat) {
    this->material = std::make_shared<PhongMaterial>(PhongMaterial());
  } else
    this->material = mat;
  this->setName(geometry->name + "_" + std::to_string(ID));
}
EMesh::~EMesh() {

  // this may cause alot of problems
  if (geometry) {
    geometry->dispose();
  }
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}

void EMesh::render(Shader &shader) {

  // Good for now, but what if each mesh or renderable object has its own shader
  if (geometry && material) {
    geometry->bind();
    shader.setMat4("uModel", getWorldMatrix());
    shader.setBool("uInstanced", instanced);
    material->Apply(shader);
    size_t idxCount = geometry->getIndiciesCount();
    if (!instanced) {
      if (idxCount) {
        glDrawElements(GL_TRIANGLES, idxCount, GL_UNSIGNED_INT, 0);

      } else
        glDrawArrays(GL_TRIANGLES, 0, 32);
    } else {
      if (idxCount) {
        glDrawElementsInstanced(GL_TRIANGLES, idxCount, GL_UNSIGNED_INT, 0,
                                instaceCount);

      } else
        glDrawArraysInstanced(GL_TRIANGLES, 0, 32, instaceCount);
    }
    geometry->unbind();
  }
  for (auto &d : children) {
    d->render(shader);
  }
}
