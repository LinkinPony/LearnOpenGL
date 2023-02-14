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
	}
	catch (...) {
		std::cerr << "Reading shader file [" << filepath << "] failed\n";
	}
	return content;
}
GLuint Shader::compileShader(GLenum shader_type, const char* const shader_source) {
	GLuint shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, &shader_source, nullptr);
	glCompileShader(shader);
	int  success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
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
	if (!success)
	{
		glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertex_shader_);
	glDeleteShader(fragment_shader_);
	return shader_program;
}
Shader::Shader(const std::string& vertex_path, const std::string& fragment_path)
{
	vertex_shader_source_ = readFile(vertex_path);
	fragment_shader_source_ = readFile(fragment_path);
	vertex_shader_ = compileShader(GL_VERTEX_SHADER, vertex_shader_source_.c_str());
	fragment_shader_ = compileShader(GL_FRAGMENT_SHADER, fragment_shader_source_.c_str());
	ID_ = createShaderProgram();
}
void Shader::use()
{
	glUseProgram(ID_);
}