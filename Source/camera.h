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

 private:
  void updateCameraStatus();
 public:
  explicit Camera(const glm::vec3 &camera_position,
                  const glm::vec3 &camera_target,
                  const glm::vec3 &up_direction);
  glm::mat4 getLookAtMat();

  void set_camera_position(const glm::vec3 &camera_position) {
    camera_position_ = camera_position;
  }
  void set_camera_target(const glm::vec3 &camera_target) {
    camera_target_ = camera_target;
  }
  void set_up_direction(const glm::vec3 &up_direction) {
    up_direction_ = up_direction;
  }
};
