#ifndef __OBJECT3D_H
#define __OBJECT3D_H
#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>
#include <vector>

class EObject3D : std::enable_shared_from_this<EObject3D> {

public:
  glm::vec3 Position;
  glm::quat Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
  glm::vec3 Scale;
  bool isObject3D = true;

  // TODO: quaternion rotation

  glm::vec3 worldUp = {0.0f, 1.0f, 0.0f};
  glm::mat4 worldModelMatrix;
  glm::mat4 localModelMatrix;

  EObject3D()
      : Position(0.0f), Rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),
        Scale(1.0f), worldModelMatrix(glm::mat4(1.0f)),
        localModelMatrix(glm::mat4(1.0f)){};

  void setRotationFromEuler(const glm::vec3 &);
  glm::vec3 getRotationAsEuler(void);
  glm::mat4 viewMatrix = glm::mat4(1.0f);

  glm::mat4 getModelViewMatrix(void);

  std::weak_ptr<EObject3D> parent;

  void traverse(std::function<void(std::shared_ptr<EObject3D>)>);

  void updateModelMatrix();
  void updateWorldMatrix();

  virtual void render() = 0;
  virtual ~EObject3D() = default;

  void add(const std::shared_ptr<EObject3D> &);
  void remove(const std::shared_ptr<EObject3D> &);

private:
  std::vector<std::shared_ptr<EObject3D>> children;
};
#endif
