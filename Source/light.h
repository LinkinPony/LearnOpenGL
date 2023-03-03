#pragma once
#include "../ThirdLib/glm/glm.hpp"
#include "../ThirdLib/glm/gtc/matrix_transform.hpp"
#include "../ThirdLib/glm/gtc/type_ptr.hpp"
#include "Shader/shader.h"
#include "model.h"
class Light {
 public:
  enum lightType { kPoint, kParallel };
  void bindUniform(const Shader& shader, int index);
  static Model getLightModel();
 private:
  lightType type_;
  float intensity_;
  glm::vec3 position_;
  glm::vec3 direction_;

  // light color
  glm::vec3 diffuse_;
  glm::vec3 specular_;
  glm::vec3 ambient_;

 private:
  static const int kMaxLightNumber = 16;

 private:
  inline static const std::string kTypeName{"type"};
  inline static const std::string kIntensityName{"intensity"};
  inline static const std::string kPositionName{"position"};
  inline static const std::string kDirectionName{"direction"};
  inline static const std::string kDiffuseName{"diffuse"};
  inline static const std::string kSpecularName{"specular"};
  inline static const std::string kAmbientName{"ambient"};
};
