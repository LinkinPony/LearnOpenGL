#include "light.h"

#include <string>
void Light::bindUniform(const Shader& shader, int index) {
  std::string pre = "light[" + std::to_string(index) + "].";

  // type
  //  TODO: map the type
  {
    int val = -1;
    if (type_ == kPoint) {
      val = 0;
    }
    if (type_ == kParallel) {
      val = 1;
    }
    shader.setUniformOneValue<int>(pre + kTypeName, val);
  }
  // intensity
  shader.setUniformOneValue<float>(pre + kIntensityName, intensity_);
  // position
  shader.setUniformVec3f(pre + kPositionName, position_);
  // direction
  shader.setUniformVec3f(pre + kDirectionName, direction_);
  // diffuse
  shader.setUniformVec3f(pre + kDiffuseName, diffuse_);
  // specular
  shader.setUniformVec3f(pre + kSpecularName, specular_);
  // ambient
  shader.setUniformVec3f(pre + kAmbientName, ambient_);
}

Model Light::getLightModel() {
  // create a cube
  std::vector<glm::vec3> pos{glm::vec3(0, 0, 0), glm::vec3(1, 0, 0),
                             glm::vec3(1, 0, 1), glm::vec3(0, 0, 1),
                             glm::vec3(0, 1, 0), glm::vec3(1, 1, 0),
                             glm::vec3(1, 1, 1), glm::vec3(0, 1, 1)};
  std::vector<unsigned int> indices{0, 1, 3, 1, 2, 3, 0, 1, 4, 1, 4, 5,
                                    0, 3, 4, 3, 4, 7, 2, 3, 6, 3, 6, 7,
                                    5, 6, 7, 4, 5, 7, 1, 2, 5, 2, 5, 6};
  std::vector<Vertex> vertex;
  for (auto& it : pos) {
    vertex.emplace_back(it, glm::vec3(0, 1, 0), glm::vec2(0, 0));
  }
  std::vector<Mesh> mesh{Mesh(vertex, indices, {})};
  return Model(mesh);
}
