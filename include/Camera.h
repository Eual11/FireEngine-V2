#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/ext/vector_float3.hpp>
enum class CameraMovement { STATIC, FORWARD, BACKWARD, LEFT, RIGHT };
class Camera {
public:
  glm::vec3 Pos = glm::vec3(0.0f, 0.0f, 0.0f);
  float fYaw = 270.0f;
  float fPitch = 0.0f;
  float fCameraSpeed = 2.5f;
  float Sensitivity = 0.1f;
  glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 Right;
  glm::vec3 Up;
  glm::vec3 Front;

  void UpdateVectors();

public:
  Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = 270.0f,
         float pitch = 0.0f) {

    Pos = pos;
    worldUp = up;
    fYaw = yaw;
    fPitch = pitch;

    UpdateVectors();
  }

  Camera(float posx, float posy, float posz, float upx, float upy, float upz,
         float fyaw, float fpitch);
  glm::mat4 getLookAt();

  void ProcessInput(CameraMovement, float deltaTime);
  void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch);

  void ProcessScroll(float yOffset);
  float Zoom = 53.;
};
#endif
