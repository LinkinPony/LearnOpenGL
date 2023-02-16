#include "camera.h"

void Camera::updateCameraStatus() {
  camera_direction_ = glm::normalize(camera_position_ - camera_target_);
  camera_right_direction_ =
      glm::normalize(glm::cross(up_direction_, camera_direction_));
  camera_up_direction_ = glm::cross(camera_direction_, camera_right_direction_);
}

Camera::Camera(const glm::vec3& camera_position, const glm::vec3& camera_target,
               const glm::vec3& up_direction)
    : camera_position_(camera_position),
      camera_target_(camera_target),
      up_direction_(up_direction) {
  updateCameraStatus();
}

glm::mat4 Camera::getLookAtMat() {
  auto e = camera_position_;
  auto g = (camera_target_ - camera_position_);
  auto t = glm::normalize(up_direction_);

  auto w = glm::normalize(g);
  auto u = glm::normalize(glm::cross(t, w));
  auto v = glm::cross(w, u);
  glm::mat4 m_translate = glm::mat4(1.0f);
  for (int i = 0; i < 3; i++) {
    m_translate[3][i] = -e[i];
  }
  glm::mat4 m_rview = glm::mat4(1.0f);
  m_rview[0] = glm::vec4(u, 0.0f);
  m_rview[1] = glm::vec4(v, 0.0f);
  m_rview[2] = glm::vec4(-w, 0.0f);
  return m_rview * m_translate;
}
