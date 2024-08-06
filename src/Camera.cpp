#include "../include/Camera.h"
#include <glm/ext/quaternion_geometric.hpp>

void Camera::UpdateVectors() {
  glm::vec3 direction;
  direction.x = cos(glm::radians(fPitch)) * cos(glm::radians(fYaw));
  direction.z = cos(glm::radians(fPitch)) * sin(glm::radians(fYaw));
  direction.y = sin(glm::radians(fPitch));

  Front = glm::normalize(direction);

  Right = glm::normalize(glm::cross(Front, worldUp));

  Up = glm::normalize(glm::cross(Right, Front));
}
glm::mat4 Camera::getLookAt() { return glm::lookAt(Pos, Pos + Front, Up); }

Camera::Camera(float posx, float posy, float posz, float upx, float upy,
               float upz, float fyaw, float fpitch) {

  Pos.x = posx;
  Pos.y = posy;
  Pos.z = posz;

  worldUp.x = upx;
  worldUp.y = upy;
  worldUp.z = upz;

  fYaw = fyaw;
  fPitch = fpitch;
  UpdateVectors();
}

void Camera::ProcessInput(CameraMovement Movement, float deltaTime) {
  float velocity = fCameraSpeed * deltaTime;

  switch (Movement) {
  case CameraMovement::FORWARD: {

    Pos += velocity * Front;
    break;
  }
  case CameraMovement::BACKWARD: {
    Pos -= velocity * Front;
    break;
  }
  case CameraMovement::LEFT: {
    Pos -= velocity * Right;
    break;
  }
  case CameraMovement::RIGHT:
    Pos += velocity * Right;
    break;
  }
}
void Camera::ProcessMouseMovement(float xOffset, float yOffset,
                                  bool constrainPitch) {
  float deltaX = xOffset * Sensitivity;
  float deltaY = yOffset * Sensitivity;

  fYaw -= deltaX;
  fPitch += deltaY;

  if (constrainPitch) {
    if (fPitch >= 89.0f)
      fPitch = 89.0f;
    if (fPitch <= -89.0f)
      fPitch = -89.0f;
  }
  UpdateVectors();
}

void Camera::ProcessScroll(float yOffset) {
  Zoom -= yOffset;

  if (Zoom >= 53.0)
    Zoom = 53.0;

  if (Zoom <= 1.0)
    Zoom = 1.0;
}
