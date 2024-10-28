#include "../include/EMesh.h"
#include <iomanip>
EMesh::EMesh(std::vector<EVertex> verts, std::vector<ETexture> texs,
             std::vector<unsigned int> indx) {

  this->geometry =
      std::make_shared<EBufferGeometry>(EBufferGeometry(verts, indx));

  this->textures = texs;
}
EMesh::EMesh(std::shared_ptr<EBufferGeometry> geo,
             std::shared_ptr<Material> mat) {
  this->geometry = geo;
  if (!mat) {
    this->material = std::make_shared<PhongMaterial>(PhongMaterial());
  } else
    this->material = mat;
}
EMesh::~EMesh() {

  // this may cause alot of problems
  /* if (geometry) { */
  /*   geometry->dispose(); */
  /* } */
  /* glDeleteVertexArrays(1, &VAO); */
  /* glDeleteBuffers(1, &VBO); */
  /* glDeleteBuffers(1, &EBO); */
}

void EMesh::render(Shader &shader) {

  if (geometry && material) {
    geometry->bind();
    shader.setMat4("uModel", getWorldMatrix());
    material->Apply(shader);
    size_t idxCount = geometry->getIndiciesCount();
    if (idxCount) {
      glDrawElements(GL_TRIANGLES, idxCount, GL_UNSIGNED_INT, 0);

    } else
      glDrawArrays(GL_TRIANGLES, 0, 32);
    geometry->unbind();
  }
}
