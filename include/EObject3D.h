#ifndef __OBJECT3D_H
#define __OBJECT3D_H
#include "Shader.h"
#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>
template <typename T> using Ref = std::shared_ptr<T>;

template <typename T, typename... Args>

std::shared_ptr<T> createRef(Args &&...args) {
  return std::make_shared<T>(std::forward<Args>(args)...);
}
class EObject3D : public std::enable_shared_from_this<EObject3D> {

public:
  enum class Type { Base, Mesh, Group, Model, Camera, World, Light };
  glm::vec3 Position;
  glm::quat Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
  glm::vec3 Scale = glm::vec3(1.0f);
  bool isObject3D = true;

  std::string name;

  // TODO: quaternion rotation

  glm::vec3 worldUp = {0.0f, 1.0f, 0.0f};
  glm::mat4 worldModelMatrix = glm::mat4(1.0f);
  glm::mat4 localModelMatrix = glm::mat4(1.0f);

  EObject3D()
      : Position(0.0f), Rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),
        Scale(1.0f), worldModelMatrix(glm::mat4(1.0f)),
        localModelMatrix(glm::mat4(1.0f)), ID(nextID++) {};

  void setRotationFromEuler(const glm::vec3 &);
  glm::vec3 getRotationAsEuler(void);
  glm::mat4 viewMatrix = glm::mat4(1.0f);

  glm::mat4 getModelViewMatrix(void);
  virtual Type getType() { return Type::Base; }

  std::weak_ptr<EObject3D> parent;

  void traverse(std::function<void(std::shared_ptr<EObject3D>)>);

  void updateModelMatrix();
  void updateWorldMatrix();
  void setPosition(float x, float y, float z);
  glm::vec3 getPosition() const { return Position; }
  void setRotation(float fPitch, float fYaw, float fRoll);
  glm::vec3 getRotation() const;

  void setScale(float x, float y, float z);

  glm::vec3 getScale() const;
  unsigned int getID() const { return ID; }

  void translate(const glm::vec3 &delta);
  void scale(const glm::vec3 &factor);
  void rotate(const glm::quat &delta);
  glm::mat4 getModelMatrix() const { return localModelMatrix; }

  glm::mat4 getWorldMatrix() const { return worldModelMatrix; }
  virtual void render(Shader &) = 0;
  void virtual setInstanceUBO(unsigned int ubo, unsigned int count) = 0;
  virtual ~EObject3D() = default;
  void lookAt(const glm::vec3 &target,
              const glm::vec3 &up = {0.0f, 1.0f, 0.0f});
  void add(const std::shared_ptr<EObject3D> &);
  void remove(const std::shared_ptr<EObject3D> &);
  void setModelMatrix(glm::mat4 transform);
  std::vector<std::shared_ptr<EObject3D>> getChildren();

protected:
  std::vector<std::shared_ptr<EObject3D>> children;
  inline static unsigned int nextID = 0;
  unsigned int ID;
};
#endif
