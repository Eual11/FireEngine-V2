#include "../include/Camera.h"
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
  glm::vec3 direction(0.0f);
  switch (Movement) {
  case CameraMovement::FORWARD:
    direction += Front;
    break;
  case CameraMovement::BACKWARD:
    direction -= Front;
    break;
  case CameraMovement::LEFT:
    direction -= Right;
    break;
  case CameraMovement::RIGHT:
    direction += Right;
    break;
  case CameraMovement::STATIC:
    // Decelerate when no movement is detected
    cameraVelocity -= acceleration * deltaTime;
    if (cameraVelocity < 0.0f) {
      cameraVelocity = 0.0f;
    }
    break;
  }

  // Normalize the direction vector only if there is movement
  if (glm::length(direction) > 0.0f) {
    direction = glm::normalize(direction);
    cameraVelocity += acceleration * 0.5 * deltaTime;
    if (cameraVelocity > max_speed) {
      cameraVelocity = max_speed;
    }
  }

  // Apply movement if there is a direction
  if (glm::length(direction) > 0.0f) {
    Pos += direction * cameraVelocity * deltaTime;
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
  Fov -= yOffset;

  if (Fov >= 53.0)
    Fov = 53.0;

  if (Fov <= 1.0)
    Fov = 1.0;
}
void Camera::UpdateUniforms(Shader &shader) {
  shader.setVec3("uViewPos", Pos);
  shader.setMat4("uView", getLookAt());
  shader.setMat4("uProjection", getProjection());
}
void Camera::SetAspectRatio(float aspect) { aspectRatio = aspect; }
glm::mat4 Camera::getProjection() {
  return glm::perspective(glm::radians(Fov), aspectRatio, zNear, zFar);
}
