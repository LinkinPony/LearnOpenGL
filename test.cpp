#include "Source/model.h"
#include "Source/Editor/editor.h"

//Model createTestModel() {
//  float raw_vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};
//
//  int vertex_num = 3;
//
//  std::vector<Vertex> vertices;
//  
//  for (int i = 0; i < vertex_num; i++) {
//    glm::vec3 position;
//    glm::vec3 normal(0, 0, 1);
//    glm::vec2 texture_coord(0, 0);
//    for (int j = 0; j < 3; j++) {
//      int idx = i * 3 + j;
//      position[j] = raw_vertices[idx];
//    }
//    vertices.emplace_back(position, normal, texture_coord);
//  }
//
//
//}


//int main() {
//    auto editor = std::make_shared<Editor>(800, 600);
//    editor->run();
//    return 0;
//  }