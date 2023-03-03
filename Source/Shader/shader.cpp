#include "shader.h"

#include <string>

std::string readFile(const std::string& filepath) {
  std::string content;
  std::ifstream file;
  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    file.open(filepath);
    std::stringstream temp_stream;
    temp_stream << file.rdbuf();
    file.close();
    content = temp_stream.str();
  } catch (...) {
    std::cerr << "Reading shader file [" << filepath << "] failed\n";
  }
  return content;
}
GLuint Shader::compileShader(GLenum shader_type,
                             const char* const shader_source) {
  GLuint shader = glCreateShader(shader_type);
  glShaderSource(shader, 1, &shader_source, nullptr);
  glCompileShader(shader);
  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char
        infoLog[BUFF_SIZE];  // store shader compile and program link error info
    glGetShaderInfoLog(shader, BUFF_SIZE, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }
  return shader;
}
GLuint Shader::createShaderProgram() {
  GLuint shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader_);
  glAttachShader(shader_program, fragment_shader_);
  glLinkProgram(shader_program);
  GLint success;
  glGetProgramiv(ID_, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[BUFF_SIZE];
    glGetProgramInfoLog(shader_program, BUFF_SIZE, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
  }
  glDeleteShader(vertex_shader_);
  glDeleteShader(fragment_shader_);
  return shader_program;
}
Shader::Shader(const std::string& vertex_path,
               const std::string& fragment_path) {
  std::string vertex_shader_source = readFile(vertex_path);
  std::string fragment_shader_source = readFile(fragment_path);
  vertex_shader_ =
      compileShader(GL_VERTEX_SHADER, vertex_shader_source.c_str());
  fragment_shader_ =
      compileShader(GL_FRAGMENT_SHADER, fragment_shader_source.c_str());
  ID_ = createShaderProgram();
}
void Shader::use() const {
  glUseProgram(ID_);
}

void Shader::setUniformVec3f(const std::string& name,
                             const glm::vec3& value) const {
  use();
  GLint location = glGetUniformLocation(ID_, name.c_str());
  glUniform3f(location, value.x, value.y, value.z);
}

void Shader::setUniformMat4f(const std::string& name, const glm::mat4& value,
                             GLboolean trans) const {
  use();
  GLint location = glGetUniformLocation(ID_, name.c_str());
  glUniformMatrix4fv(location, 1, trans, &value[0][0]);
}
