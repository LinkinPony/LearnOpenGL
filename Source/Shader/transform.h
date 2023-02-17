#pragma once 
#include "../../ThirdLib/glm/glm.hpp"
#include "../../ThirdLib/glm/gtc/matrix_transform.hpp"
#include "../../ThirdLib/glm/gtc/type_ptr.hpp"

class Transform {
 public:
  // rotate by axis. degree version and radian version.
  static glm::mat4 rotateByRadian(float radian, const glm::vec3& axis);
  static glm::mat4 rotateByDegree(float degree, const glm::vec3& axis);
  // multiply x,y,z by times.
  static glm::mat4 scale(float times);
  static glm::mat4 translation(const glm::vec3& translate);

  static glm::mat4 modelTrans(float rotate_degree, const glm::vec3& rotate_axis,
                              float scale_times, const glm::vec3& translate);
};
