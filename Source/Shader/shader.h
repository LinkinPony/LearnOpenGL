#pragma once
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
class Shader
{
private:
	GLuint ID_;
	std::string vertex_shader_source_;
	std::string fragment_shader_source_;
	GLuint vertex_shader_;
	GLuint fragment_shader_;
	
private:
	static constexpr int BUFF_SIZE = 512;
	char infoLog[BUFF_SIZE];//store shader compile and program link error info
	GLuint compileShader(GLenum shader_type, const char* const shader_source);
	GLuint createShaderProgram();
public:
	GLuint get_ID() { return ID_; }

	Shader(const std::string& vertex_path, const std::string& fragment_path);

	void use();
	template <typename T>
	void setUniformValue(const std::string& name, const T &value) const;

};

template<typename T>
inline void Shader::setUniformValue(const std::string& name, const T& value) const
{
	GLint location = glGetUniformLocation(ID_, name.c_str());
	glUniform1i(location, value);
}
