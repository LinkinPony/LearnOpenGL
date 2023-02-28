#pragma once
#include <memory>
#include <string>
#include <map>
#include <vector>
// GLAD, include it before GLFW
#include <glad/glad.h>
// GLFW
#include <GLFW/glfw3.h>

#include "../ThirdLib/glm/glm.hpp"
#include "../ThirdLib/glm/gtc/matrix_transform.hpp"
#include "../ThirdLib/glm/gtc/type_ptr.hpp"
#include "Shader/shader.h"
struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec3 tex_coord;
};
struct Texture {
  enum texture_type { kDiffuse, kSpecular };
  //use type2name to get texture's name in shader.
  //kDiffuse = "diffuse", kSpecular = "specular"
  //name rule in shader: type T's i-th texture is
  //material.type2name[T]_i
  //for example, 3th(numbered from zero) specular texture's name is
  //material.specular_2
  const static std::map<texture_type, std::string> type2name;
  GLuint id;
  texture_type type;
};
class Mesh {
 public:
  std::vector<Vertex> vertices_;
  std::vector<unsigned int> indices_;
  std::vector<Texture> textures_;

  Mesh(const std::vector<Vertex> &vertices,
       const std::vector<unsigned int> &indices,
       const std::vector<Texture> &textures);
  ~Mesh();
  // TODO: finish all constructor

  void draw(std::shared_ptr<Shader> shader);

 private:
  GLuint VAO_;
  GLuint VBO_;
  GLuint EBO_;

 private:
  void init();
  void initTexture(std::shared_ptr<Shader> shader);
};
