#include "camera.h"
void Camera::updateCameraStatus() {
  // camera_direction_ = glm::normalize(camera_direction_);
  camera_target_ = camera_position_ + camera_direction_;
  camera_right_direction_ =
      glm::normalize(glm::cross(up_direction_, camera_direction_));
  camera_up_direction_ = glm::cross(camera_direction_, camera_right_direction_);
}

Camera::Camera(const glm::vec3& camera_position,
               const glm::vec3& camera_direction, const glm::vec3& up_direction)
    : camera_position_(camera_position),
      camera_direction_(glm::normalize(camera_direction)),
      up_direction_(glm::normalize(up_direction)) {
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
    m_translate[i][3] = -e[i];
  }
  glm::mat4 m_rview = glm::mat4(1.0f);
  m_rview[0] = glm::vec4(u, 0.0f);
  m_rview[1] = glm::vec4(v, 0.0f);
  m_rview[2] = glm::vec4(-w, 0.0f);
  return m_rview * m_translate;
}

void Camera::moveByDirection(MoveDirection direction) {
  auto cam_pos = get_camera_position();
  float speed = get_move_speed();
  switch (direction) {
    case Camera::MoveDirection::kLeft:
      cam_pos += speed * get_camera_right_direction();
      break;
    case Camera::MoveDirection::kRight:
      cam_pos -= speed * get_camera_right_direction();
      break;
    case Camera::MoveDirection::kForward:
      cam_pos += speed * get_camera_direction();
      break;
    case Camera::MoveDirection::kBack:
      cam_pos -= speed * get_camera_direction();
      break;
    case Camera::MoveDirection::kUp:
      cam_pos += speed * get_up_direction();
      break;
    case Camera::MoveDirection::kDown:
      cam_pos -= speed * get_up_direction();
      break;
    default:
      break;
  }
  set_camera_position(cam_pos);
}

void Camera::moveByEulerianAngles(float pitch_degree, float yaw_degree) {
  glm::vec3 direction;
  float pitch_radian = glm::radians(pitch_degree);
  float yaw_radian = glm::radians(yaw_degree);
  direction.x = -cos(pitch_radian) * cos(yaw_radian);
  direction.y = -sin(pitch_radian);
  direction.z = cos(pitch_radian) * sin(yaw_radian);
  direction = glm::normalize(direction);
  set_camera_direction(direction);
}

void Camera::moveByEulerianAngles() { 
    moveByEulerianAngles(pitch_, yaw_); }
