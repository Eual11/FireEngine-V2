#include "../include/EObject3D.h"

void EObject3D::setRotationFromEuler(const glm::vec3 &rot) {
  Rotation = glm::quat(glm::radians(rot));
  updateModelMatrix();
}

void EObject3D::traverse(std::function<void(std::shared_ptr<EObject3D>)> func) {
  func(shared_from_this());
  for (auto &d : children) {
    d->traverse(func);
  }
}

glm::mat4 EObject3D::getModelViewMatrix(void) {
  return viewMatrix * worldModelMatrix;
}

void EObject3D::updateModelMatrix() {
  localModelMatrix = glm::translate(glm::mat4(1.0f), Position) *
                     glm::mat4_cast(Rotation) *
                     glm::scale(glm::mat4(1.0f), Scale);
}

void EObject3D::updateWorldMatrix() {
  updateModelMatrix();

  if (auto p = this->parent.lock()) {
    worldModelMatrix = p->worldModelMatrix * localModelMatrix;
  } else {
    worldModelMatrix = localModelMatrix;
  }

  for (auto &c : children) {
    c->updateWorldMatrix();
  }
}
void EObject3D::add(const std::shared_ptr<EObject3D> &child) {
  child->parent = shared_from_this();

  children.push_back(child);
}
void EObject3D::remove(const std::shared_ptr<EObject3D> &child) {
  auto it = std::find(children.begin(), children.end(), child);

  if (it != children.end()) {
    (*it)->parent.reset();
    children.erase(it);
  }
}
