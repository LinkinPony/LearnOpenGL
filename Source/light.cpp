#include "light.h"

#include <string>
void Light::bindUniform(const Shader& shader, int index) {
  std::string pre = "light[" + std::to_string(index) + "].";

  //type
  // TODO: map the type
  { int val = -1;
    if (type_ == kPoint) {
      val = 0;
    }
    if (type_ == kParallel) {
      val = 1;
    }
    shader.setUniformOneValue<int>(pre + kTypeName,val);
  }
  //intensity
  shader.setUniformOneValue<float>(pre + kIntensityName, intensity_);
  //position
  shader.setUniformVec3f(pre + kPositionName, position_);
  //direction
  shader.setUniformVec3f(pre + kDirectionName, direction_);
  //diffuse
  shader.setUniformVec3f(pre + kDiffuseName, diffuse_);
  //specular
  shader.setUniformVec3f(pre + kSpecularName, specular_);
  //ambient
  shader.setUniformVec3f(pre + kAmbientName, ambient_);
}
