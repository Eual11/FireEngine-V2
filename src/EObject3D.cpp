#include "../include/EObject3D.h"
#include <cstdio>
#include <glm/gtc/quaternion.hpp>
#include <glm/trigonometric.hpp>
#include <iomanip>

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

  updateWorldMatrix();
}
void prettyPrintMat4(const glm::mat4 &mat) {
  std::cout << std::fixed
            << std::setprecision(2); // Set precision to 2 decimal places
  for (int row = 0; row < 4; ++row) {
    std::cout << "| ";
    for (int col = 0; col < 4; ++col) {
      std::cout << std::setw(8) << mat[col][row]
                << " "; // Set width for spacing
    }
    std::cout << "|\n";
  }
  std::cout << std::endl; // Extra newline for spacing
}
void EObject3D::updateWorldMatrix() {

  if (auto p = this->parent.lock()) {
    // maybe this should be reversed
    worldModelMatrix = p->getWorldMatrix() * localModelMatrix;

  } else {
    worldModelMatrix = localModelMatrix;
  }

  for (auto &c : children) {
    c->updateWorldMatrix();
  }
}

void EObject3D::setModelMatrix(glm::mat4 transform) {
  localModelMatrix = transform;
  updateWorldMatrix();
}
void EObject3D::add(const std::shared_ptr<EObject3D> &child) {
  if (child) {
    if (child.get() == this) {
      throw std::logic_error("EObject3D can't be a child of itself\n");
      return;
    }
    if (parent.lock() == child) {
      throw std::logic_error(
          "Cannot add a parent objects as a child, creating circular "
          "dependency\n ");
      return;
    }

    try {
      child->parent = shared_from_this();
    } catch (const std::bad_weak_ptr &) {
      printf("Bad A weak ptr\n");
    }
    children.push_back(child);
    child->updateWorldMatrix();
  }
}
void EObject3D::remove(const std::shared_ptr<EObject3D> &child) {
  auto it = std::find(children.begin(), children.end(), child);

  if (it != children.end()) {
    (*it)->parent.reset();
    children.erase(it);
  }
}
void EObject3D::setPosition(float x, float y, float z) {
  Position = {x, y, z};
  updateModelMatrix();
}
void EObject3D::setRotation(float fPitch, float fYaw, float fRoll) {
  Rotation = glm::quat(glm::radians(glm::vec3(fPitch, fYaw, fRoll)));
  updateModelMatrix();
}
glm::vec3 EObject3D::getRotation() const {
  return glm::degrees(glm::eulerAngles(Rotation));
}
void EObject3D::setScale(float x, float y, float z) {
  Scale = {x, y, z};
  updateModelMatrix();
}
glm::vec3 EObject3D::getScale() const { return Scale; }

void EObject3D::translate(const glm::vec3 &delta) {
  Position += delta;
  updateModelMatrix();
}
void EObject3D::scale(const glm::vec3 &factor) {
  Scale *= factor;
  updateModelMatrix();
}
void EObject3D::rotate(const glm::quat &delta) {
  Rotation = delta * Rotation;
  updateModelMatrix();
}
void EObject3D::lookAt(const glm::vec3 &target, const glm::vec3 &up) {
  glm::mat4 lookAtMatrix = glm::lookAt(Position, target, up);
  Rotation = glm::quat_cast(glm::inverse(lookAtMatrix));
  updateModelMatrix();
}
std::vector<std::shared_ptr<EObject3D>> EObject3D::getChildren() {
  return children;
}
