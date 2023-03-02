#include "mesh.h"
const static std::map<Texture::texture_type, std::string> type2name = {
    {Texture::texture_type::kDiffuse, "diffuse"},
    {Texture::texture_type::kSpecular, "specular"}};
Mesh::Mesh(const std::vector<Vertex> vertices,
           const std::vector<unsigned int> indices,
           const std::vector<Texture> textures)
    : vertices_(vertices), indices_(indices), textures_(textures) {
  VAO_ = VBO_ = EBO_ = 0;
  init();
}

Mesh::~Mesh() {
  // TODO: clean up resources
}

void Mesh::draw(const Shader &shader) {
  shader.use();
  // init texture
  //init();
  initTexture(shader);
  //std::cout << "vertex: " << vertices_.size() << std::endl;
  //std::cout << "indicies: " << indices_.size() << std::endl;
  //draw
  glBindVertexArray(VAO_);
  glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices_.size()), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void Mesh::init() {
  // generate buffers
  glGenVertexArrays(1, &VAO_);
  glGenBuffers(1, &VBO_);
  glGenBuffers(1, &EBO_);
  // bind data to buffers
  glBindVertexArray(VAO_);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_);
  glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex),
               vertices_.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int),
               indices_.data(), GL_STATIC_DRAW);

  // Bind data to vertex shader
  // position
  
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)(0));
  glEnableVertexAttribArray(0);
  // normal
  
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)(offsetof(Vertex, normal)));
  glEnableVertexAttribArray(1);
  // texture coordinate
  
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)(offsetof(Vertex, tex_coord)));
  glEnableVertexAttribArray(2);
  // reset status
  glBindVertexArray(0);
}

void Mesh::initTexture(const Shader & shader) {
  std::map<Texture::texture_type, GLuint> type_cnt;
  for (GLuint texture_cnt = 0; texture_cnt < textures_.size(); texture_cnt++) {
    auto &it = textures_[texture_cnt];
    glActiveTexture(GL_TEXTURE0 + texture_cnt);
    std::string name = "texture_" + type2name.find(it.type)->second + "_";
    name += std::to_string(type_cnt[it.type]++);
    //std::cout << "set texture [" << name << "]" << std::endl;
    shader.setUniformOneValue<GLuint>(name, texture_cnt);
    glBindTexture(GL_TEXTURE_2D, textures_[texture_cnt].id);
  }
}
