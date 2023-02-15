#include "transform.h"

glm::mat4 Transform::rotateByRadian(float radian, const glm::vec3& axis) {
  auto m_rotate = glm::mat4(1.0f);
  return glm::rotate(m_rotate, radian, axis);
}

glm::mat4 Transform::rotateByDegree(float degree, const glm::vec3& axis) {
  auto m_rotate = glm::mat4(1.0f);
  return glm::rotate(m_rotate, glm::radians(degree), axis);
}

glm::mat4 Transform::scale(float times) {
  auto m_scale = glm::mat4(1.0f);
  return glm::scale(m_scale, glm::vec3(times, times, times));
}

glm::mat4 Transform::translation(const glm::vec3& translate) {
  glm::mat4 identity = glm::mat4(1.0f);
  return glm::translate(identity, translate);
}

glm::mat4 Transform::modelTrans(float rotate_degree,
                                const glm::vec3& rotate_axis, float scale_times,
                                const glm::vec3& translate) {
  return rotateByDegree(rotate_degree, rotate_axis) * scale(scale_times) *
         translation(translate);
}
