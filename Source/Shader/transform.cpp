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

glm::mat4 Transform::orthoProjection(float l, float r, float b, float t,
                                     float f, float n) {
  glm::mat4 m_ortho_l = glm::mat4(1.0f);
  m_ortho_l[0][0] = 2 / (r - l);
  m_ortho_l[1][1] = 2 / (t - b);
  m_ortho_l[2][2] = 2 / (n - f);
  
  glm::mat4 m_ortho_r = glm::mat4(1.0f);
  m_ortho_r[0][3] = - (l + r) / 2;
  m_ortho_r[1][3] = - (b + t) / 2;
  m_ortho_r[2][3] = - (f + n) / 2;

  return m_ortho_l * m_ortho_r;
}

glm::mat4 Transform::perspProjection(float n, float f) { 
    glm::mat4 m_persp = glm::mat4(1.0f);
  m_persp[0][0] = n;
  m_persp[1][1] = n;
  m_persp[2][2] = n + f;
  m_persp[2][3] = - n * f;
  m_persp[3][2] = 1;
  m_persp[3][3] = 0;
  return m_persp;
}

glm::mat4 Transform::projectionTrans(float fov_degree, float aspect_ratio,
                                     float z_near, float z_far) {
  float halfV = glm::radians(fov_degree / 2);
  float n = z_near;
  float f = z_far;
  float t = n * tan(halfV);
  float r = aspect_ratio * t;


  auto result = glm::mat4();
  result[0][0] = n / r;
  result[1][1] = n / t;
  result[2][2] = (n + f) / (n - f);
  result[2][3] = (2 * n * f) / (n - f);
  result[3][2] = -1;
  return result;
}

glm::mat4 Transform::modelTrans(float rotate_degree,
                                const glm::vec3& rotate_axis, float scale_times,
                                const glm::vec3& translate) {
  // do rotate first, then translation.
  return translation(translate) * scale(scale_times) *
         rotateByDegree(rotate_degree, rotate_axis);
}