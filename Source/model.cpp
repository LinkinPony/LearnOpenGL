#include "model.h"
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "../ThirdLib/stb_image.h"
#endif

std::vector<Texture> Model::loadMaterialTexture(aiMaterial* material,
                                                aiTextureType ai_type,
                                                Texture::texture_type type) {
  std::vector<Texture> textures;
  for (unsigned int i = 0; i < material->GetTextureCount(ai_type); i++) {
    aiString ai_path;
    material->GetTexture(ai_type, i, &ai_path);
    std::string path(ai_path.C_Str());
    GLuint id = loadTextureFromFile(path.c_str(), directory_);
    textures.emplace_back(id, path, type);
  }
  return textures;
}

GLuint Model::loadTextureFromFile(const std::string& path,
                                  const std::string& directory) {
  std::string filename = directory + "/" + path;
  GLuint texture_id;
  glGenTextures(1, &texture_id);
  int width, height, nrChannels;
  unsigned char* data =
      stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
  if (data) {
    std::cout << "Texture width x height = " << width << " x " << height
              << std::endl;
    GLenum format;
    switch (nrChannels) {
      case 1:
        format = GL_RED;
        break;
      case 3:
        format = GL_RGB;
        break;
      case 4:
        format = GL_RGBA;
        break;
      default:
        format = GL_RGB;
        break;
    }
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  } else {
    std::cout << "Failed to load texture " << filename << std::endl;
  }
  stbi_image_free(data);
  return texture_id;
}

Model::Model(const std::string& filepath) { loadModel(filepath); }

Model::Model(const std::vector<Mesh>& mesh) : mesh_(mesh) {}

void Model::draw(const Shader& shader) {
  shader.use();
  shader.setUniformMat4f("m_model", m_model_);
  for (auto& it : mesh_) {
    it.draw(shader);
  }
}

void Model::loadModel(const std::string& path) {
  Assimp::Importer importer;
  auto scene = importer.ReadFile(
      path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
  if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) ||
      !(scene->mRootNode)) {
    std::cout << "[error] assimp: " << importer.GetErrorString() << std::endl;
  } else {
    std::cout << "assimp loaded model" << path << std::endl;
  }
  // TODO : change this symbol '/'
  directory_ = path.substr(0, path.find_last_of('/'));
  if (scene) {
    processNode(scene->mRootNode, scene);
  }
}

void Model::processNode(aiNode* node, const aiScene* scene) {
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    auto mesh_ptr = scene->mMeshes[node->mMeshes[i]];
    mesh_.push_back(processMesh(mesh_ptr, scene));
  }
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    auto node_ptr = node->mChildren[i];
    processNode(node_ptr, scene);
  }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;
  // process vertex
  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    glm::vec3 pos(mesh->mVertices[i].x, mesh->mVertices[i].y,
                  mesh->mVertices[i].z);
    // TODO: change this
    assert(mesh->HasNormals());
    glm::vec3 norm(mesh->mNormals[i].x, mesh->mNormals[i].y,
                   mesh->mNormals[i].z);
    auto tex_coord = glm::vec2(0.0f, 0.0f);
    if (mesh->mTextureCoords[0]) {
      tex_coord =
          glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
    }
    vertices.emplace_back(Vertex(pos, norm, tex_coord));
  }
  // process index
  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    auto& face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }
  // process texture
  if (mesh->mMaterialIndex >= 0) {
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    auto diffuse_maps =
        loadMaterialTexture(material, aiTextureType_DIFFUSE, Texture::kDiffuse);
    textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());
    auto specular_maps = loadMaterialTexture(material, aiTextureType_SPECULAR,
                                             Texture::kSpecular);
    textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
  }
  return Mesh(vertices, indices, textures);
}
