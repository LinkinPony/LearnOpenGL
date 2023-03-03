#pragma once
#include "../ThirdLib/glm/glm.hpp"
#include "../ThirdLib/glm/gtc/matrix_transform.hpp"
#include "../ThirdLib/glm/gtc/type_ptr.hpp"
#include "Shader/shader.h"
#include "model.h"
class Light {
 public:
  enum lightType { kPoint, kParallel };
  void bindUniform(const Shader &shader, int index);
  static Model generateLightModel();
  Light(lightType type, float intensity, const glm::vec3 &position,
        const glm::vec3 &direction, const glm::vec3 &diffuse,
        const glm::vec3 &specular, const glm::vec3 &ambient);

 public:
  glm::mat4 get_m_model();
  void set_intensity(float intensity) { intensity_ = intensity; }
  void set_position(glm::vec3 &position) { position_ = position; }
  void set_direction(glm::vec3 &direction) { direction_ = direction; }
  void set_diffuse(glm::vec3 &diffuse_color) { diffuse_ = diffuse_color; }
  void set_specular(glm::vec3 &specular_color) { specular_ = specular_color; }
  void set_ambient(glm::vec3 &ambient_color) { ambient_ = ambient_color; }

  Model &get_model() { 
      light_model_.set_m_model(get_m_model());
      return light_model_; 
  }
 private:
  lightType type_;
  float intensity_;
  glm::vec3 position_;
  glm::vec3 direction_;
  Model light_model_;

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
