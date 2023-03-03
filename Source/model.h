#pragma once
#include <string>
#include <vector>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include "mesh.h"
class Model {
 public:
  Model(const std::string& filepath);
  Model(const std::vector<Mesh>& mesh);
  void draw(const Shader & shader);
 private:
  std::vector<Mesh> mesh_;
  std::string directory_;

 private:
  void loadModel(const std::string& path);
  void processNode(aiNode * node,
                   const aiScene * scene);
  Mesh processMesh(aiMesh * mesh,
                   const aiScene * scene);
  std::vector<Texture> loadMaterialTexture(aiMaterial* material,
                                           aiTextureType ai_type,
                                           Texture::texture_type type);
  GLuint loadTextureFromFile(const std::string& path,
                             const std::string& directory);
};
