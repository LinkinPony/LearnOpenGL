#pragma once
#include <glad/glad.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "../../ThirdLib/glm/glm.hpp"
#include "../../ThirdLib/glm/gtc/matrix_transform.hpp"
#include "../../ThirdLib/glm/gtc/type_ptr.hpp"
class Shader {
 private:
  GLuint ID_;
  GLuint vertex_shader_;
  GLuint fragment_shader_;

 private:
  static constexpr int BUFF_SIZE = 512;

  GLuint compileShader(GLenum shader_type, const char* const shader_source);
  GLuint createShaderProgram();

 public:
  Shader(const std::string& vertex_path, const std::string& fragment_path);
  Shader() {
    ID_ = NULL;
    fragment_shader_ = NULL;
    vertex_shader_ = NULL;
  }
  // we want Shader act like unique_ptr, so we can relese shader program easily.
  Shader(const Shader&) = delete;
  Shader& operator=(const Shader&) = delete;
  Shader& operator=(Shader&& rhs) noexcept {
    if (this != &rhs) {
      ID_ = rhs.ID_;
      vertex_shader_ = rhs.vertex_shader_;
      fragment_shader_ = rhs.fragment_shader_;
      rhs.ID_ = rhs.vertex_shader_ = rhs.fragment_shader_ = 0;
    }
    return *this;
  }
  Shader(Shader&& rhs) noexcept
      : ID_(rhs.ID_),
        vertex_shader_(rhs.vertex_shader_),
        fragment_shader_(rhs.fragment_shader_) {
    rhs.ID_ = rhs.vertex_shader_ = rhs.fragment_shader_ = 0;
  }

  ~Shader() {
    glDeleteProgram(ID_);
    glDeleteShader(vertex_shader_);
    glDeleteShader(fragment_shader_);
  }

 public:
  GLuint get_ID() { return ID_; }
  void use() const;
  template <typename T>
  void setUniformOneValue(const std::string& name, const T& value) const;
  void setUniformVec3f(const std::string& name, const glm::vec3& value) const;
  void setUniformMat4f(const std::string& name, const glm::mat4& value,
                       GLboolean trans = GL_FALSE) const;
};

template <typename T>
inline void Shader::setUniformOneValue(const std::string& name,
                                       const T& value) const {
  GLint location = glGetUniformLocation(ID_, name.c_str());
  glUniform1i(location, value);
}
