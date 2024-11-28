#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "EObject3D.h"
#include "Shader.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_geometric.hpp>

#include <glm/ext/vector_float3.hpp>
enum class CameraMovement { STATIC, FORWARD, BACKWARD, LEFT, RIGHT };
class Camera : public EObject3D {
public:
  /* glm::vec3 Pos = glm::vec3(0.0f, 0.0f, 0.0f); */
  float fYaw = 270.0f;
  float fPitch = 0.0f;
  float max_speed = 10.0f;
  float acceleration = 20.0f;
  float cameraVelocity = 0.0f;
  float Sensitivity = 0.1f;
  float zFar = 100.0f;
  float zNear = 0.1;
  float aspectRatio = 16.0f / 9.0f;
  glm::vec3 Right;
  glm::vec3 Up;
  glm::vec3 Front;
  float Fov = 53.0f;

  bool isCamera = true;
  void UpdateVectors();

public:
  Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = 270.0f,
         float pitch = 0.0f, float far = 100.0f, float near = 0.1f,
         float fov = 45) {

    Position = pos;
    worldUp = up;
    fYaw = yaw;
    fPitch = pitch;
    zFar = far;
    zNear = near;
    Fov = fov;

    UpdateVectors();
  }

  Camera(float posx, float posy, float posz, float upx, float upy, float upz,
         float fyaw, float fpitch);
  glm::mat4 getLookAt();
  glm::mat4 getProjection();
  Camera::Type getType() override { return Type::Camera; }

  void ProcessInput(CameraMovement, float deltaTime);
  void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch);

  void ProcessScroll(float yOffset);
  void SetAspectRatio(float aspect);
  void UpdateUniforms(Shader &shader);
  void render(Shader &) override;
  void setInstanceUBO(unsigned int ubo, unsigned int count) override {};
};

#endif
