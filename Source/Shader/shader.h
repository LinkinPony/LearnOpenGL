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
  std::string vertex_shader_source_;
  std::string fragment_shader_source_;
  GLuint vertex_shader_;
  GLuint fragment_shader_;

 private:
  static constexpr int BUFF_SIZE = 512;
  char infoLog[BUFF_SIZE];  // store shader compile and program link error info
  GLuint compileShader(GLenum shader_type, const char* const shader_source);
  GLuint createShaderProgram();

 public:
  GLuint get_ID() { return ID_; }

  Shader(const std::string& vertex_path, const std::string& fragment_path);
  Shader() {
    ID_ = NULL;
    fragment_shader_ = NULL;
    vertex_shader_ = NULL;
    memset(infoLog, 0, sizeof(infoLog));
  }
  ~Shader() { glDeleteProgram(ID_); }
  void use();
  template <typename T>
  void setUniformOneValue(const std::string& name, const T& value) const;
  void setUniformVec3f(const std::string& name, const glm::vec3& value) const;
};

template <typename T>
inline void Shader::setUniformOneValue(const std::string& name,
                                       const T& value) const {
  GLint location = glGetUniformLocation(ID_, name.c_str());
  glUniform1i(location, value);
}
