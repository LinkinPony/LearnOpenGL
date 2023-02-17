#pragma once
#include "../ThirdLib/glm/glm.hpp"
#include "../ThirdLib/glm/gtc/matrix_transform.hpp"
#include "../ThirdLib/glm/gtc/type_ptr.hpp"
class Camera {
 private:
  glm::vec3 camera_position_;
  glm::vec3 camera_target_;
  glm::vec3 camera_direction_;
  glm::vec3 up_direction_;
  glm::vec3 camera_right_direction_;
  glm::vec3 camera_up_direction_;

  float move_speed_;

 private:
  void updateCameraStatus();

 public:
  enum class MoveDirection { kLeft, kRight, kForward, kBack, kUp, kDown };

 public:
  explicit Camera(const glm::vec3 &camera_position,
                  const glm::vec3 &camera_target,
                  const glm::vec3 &up_direction);
  glm::mat4 getLookAtMat();
  void moveByDirection(MoveDirection direction);

 public:
  void set_camera_position(const glm::vec3 &camera_position) {
    camera_position_ = camera_position;
    updateCameraStatus();
  }
  void set_camera_target(const glm::vec3 &camera_target) {
    camera_target_ = camera_target;
    updateCameraStatus();
  }
  void set_up_direction(const glm::vec3 &up_direction) {
    up_direction_ = up_direction;
    updateCameraStatus();
  }
  void set_move_speed(float speed) { move_speed_ = speed; }

 public:
  glm::vec3 get_camera_position() { return camera_position_; }
  glm::vec3 get_camera_target() { return camera_target_; }
  glm::vec3 get_camera_direction() { return camera_direction_; }
  glm::vec3 get_up_direction() { return up_direction_; }
  glm::vec3 get_camera_right_direction() { return camera_right_direction_; }
  glm::vec3 get_camera_up_direction() { return camera_up_direction_; }
  float get_move_speed() { return move_speed_; }
};
