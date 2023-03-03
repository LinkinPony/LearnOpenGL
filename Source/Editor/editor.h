#pragma once
#include <iostream>
#include <vector>
// GLAD, include it before GLFW
#include <glad/glad.h>
// GLFW
#include <GLFW/glfw3.h>

#include <string>

#include "../Shader/shader.h"
#include "../Shader/transform.h"
#include "../camera.h"
#include "../light.h"
#include "../model.h"

class Editor : public std::enable_shared_from_this<Editor> {
 private:
  int initEnvironment();
  void initCamera();
  void initShader();
  void loadModel();
  void processKeyBoardInput(GLFWwindow* window);
  void mouseCallback(double xpos, double ypos);
  void scrollCallback(double xoffset, double yoffset);
 private:
  int screen_width_;
  int screen_height_;
  float fov_ = 45;
  Camera camera_;
  Shader shader_;
  GLFWwindow* window_;

 private:
  std::vector<Model> model_;
  std::vector<Light> light_;
 public:
  explicit Editor(int screen_width, int screen_height);
  int run();
  friend void glmouseCallbackWrapper(GLFWwindow* window, double xpos,
                                     double ypos);
  friend void glscrollCallbackWrapper(GLFWwindow* window, double xoffset,
                                      double yoffset);
};
