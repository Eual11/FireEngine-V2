#include "../include/EModel.h"
#include <memory>

// NOTE: this hierarchy structure of Model is very bad and wastes alot of
// memory
// TODO: GET A BETTER STRUCTURE
void EModel::render(Shader &shader) {
  if (material) {
    material->Apply(shader);
  }
  for (auto &child : children) {
    child->render(shader);
  }
}
void EModel::enableInstanced(const std::vector<glm::mat4> &matrices) {
  instanced = true;
  unsigned int instanceCount = matrices.size();

  unsigned int instaBuffer;
  glGenBuffers(1, &instaBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, instaBuffer);

  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * instanceCount, &matrices[0],
               GL_STATIC_DRAW);

  for (auto &child : children) {

    child->setInstanceUBO(instaBuffer, instanceCount);
  }
}
EModel::Type EModel::getType() { return EModel::Type::Model; }
