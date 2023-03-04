#include "editor.h"

#include "stb_image.h"
static std::shared_ptr<Editor> editor;  // for gl function wrapper

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
  return;
}
void glmouseCallbackWrapper(GLFWwindow* window, double xpos, double ypos) {
  if (editor == nullptr) return;
  editor->mouseCallback(xpos, ypos);
}

void glscrollCallbackWrapper(GLFWwindow* window, double xoffset,
                             double yoffset) {
  if (editor == nullptr) return;
  editor->scrollCallback(xoffset, yoffset);
}

int Editor::initEnvironment() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  window_ = glfwCreateWindow(screen_width_, screen_height_, "Demo window",
                             nullptr, nullptr);
  if (window_ == nullptr) {
    std::cout << "Create window failed." << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window_);
  glfwSetFramebufferSizeCallback(window_, framebufferSizeCallback);
  // disable mouse cursor and capture it
  glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window_, glmouseCallbackWrapper);
  glfwSetScrollCallback(window_, glscrollCallbackWrapper);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  stbi_set_flip_vertically_on_load(true);
  glEnable(GL_DEPTH_TEST);
  return 0;
}

void Editor::initCamera() {
  auto camera_position = glm::vec3(0, 0, 3);
  auto camera_direction = glm::vec3(0, 0, -1);
  auto up_direction = glm::vec3(0, 1, 0);
  camera_ = Camera(camera_position, camera_direction, up_direction);
  camera_.set_move_speed(0.05);
}

void Editor::initShader() {
  shader_ = Shader("Source/ShaderSource/vertex_shader.vert",
                   "Source/ShaderSource/fragment_shader.frag");
  light_shader_ = Shader("Source/ShaderSource/vertex_shader.vert",
                         "Source/ShaderSource/light_shader.frag");
}

void Editor::loadLight() {
  float intensity = 100;
  auto position = glm::vec3(4, 4, 4);
  auto color = glm::vec3(1, 1, 1);
  Light light_1(Light::kPoint, intensity, position, glm::vec3(0, 0, 0), color,
                color, color);
  light_.push_back(light_1);
}

void Editor::configueLight() {
  shader_.use();
  shader_.setUniformOneValue<GLint>("light_num",
                                    static_cast<GLint>(light_.size()));
  shader_.setUniformVec3f("eye_direction", -camera_.get_camera_direction());
  float cur_time = glfwGetTime();
  for (size_t i = 0; i < light_.size(); i++) {
    light_shader_.setUniformOneValue<GLint>("index", i);
    light_[i].set_diffuse(glm::vec3(sin(cur_time),cos(cur_time),sin(cur_time) * cos(cur_time)));
    light_[i].bindUniform(shader_, i);
    light_[i].bindUniform(light_shader_, i);
    light_[i].get_model().draw(light_shader_);
  }
}

void Editor::loadModel() {
  auto m_model = Transform::modelTrans(0.0f, glm::vec3(1, 0.3, 0.5), 1,
                                       glm::vec3(0, 0, 0));
  auto model_1 = Model("Resource/Model/nanosuit/nanosuit.obj ");
  model_1.set_m_model(m_model);
  model_.push_back(model_1);
}

Editor::Editor(int screen_width, int screen_height)
    : screen_height_(screen_height), screen_width_(screen_width) {
  window_ = 0;
}

void debugPrintMat4(const glm::mat4& mat) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      std::cout << mat[i][j];
      if (j == 3)
        std::cout << std::endl;
      else
        std::cout << " ";
    }
  }
}

int Editor::run() {
  editor = this->shared_from_this();
  int exit_code = initEnvironment();
  if (exit_code) {
    return exit_code;
  }
  initCamera();
  initShader();
  loadModel();
  loadLight();
  float last_time = 0;
  float delta_time = 0;
  while (!glfwWindowShouldClose(window_)) {
    // render process
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float cur_time = glfwGetTime();
    delta_time = cur_time - last_time;
    last_time = cur_time;
    camera_.set_move_speed(delta_time * 5);
    processKeyBoardInput(window_);
    /*TRANSFORM BEGIN*/
    camera_.moveByEulerianAngles();
    auto m_view = camera_.getLookAtMat();
    auto m_projection = Transform::projectionTrans(
        fov_, static_cast<float>(screen_width_) / screen_height_, 0.1f, 100.0f);
    /*TRANSFORM END*/
    shader_.use();
    shader_.setUniformMat4f("m_view", m_view, GL_TRUE);
    shader_.setUniformMat4f("m_projection", m_projection, GL_TRUE);
    light_shader_.use();
    light_shader_.setUniformMat4f("m_view", m_view, GL_TRUE);
    light_shader_.setUniformMat4f("m_projection", m_projection, GL_TRUE);
    configueLight();
    // draw all models
    for (auto& it : model_) {
      it.draw(shader_);
    }
    glfwSwapBuffers(window_);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}

void Editor::processKeyBoardInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera_.moveByDirection(Camera::MoveDirection::kForward);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera_.moveByDirection(Camera::MoveDirection::kLeft);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera_.moveByDirection(Camera::MoveDirection::kBack);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera_.moveByDirection(Camera::MoveDirection::kRight);
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    camera_.moveByDirection(Camera::MoveDirection::kUp);
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    camera_.moveByDirection(Camera::MoveDirection::kDown);
  }
}

void Editor::mouseCallback(double xpos, double ypos) {
  static float last_x = screen_width_ / 2;
  static float last_y = screen_height_ / 2;
  float xoffset = xpos - last_x;
  float yoffset = last_y - ypos;
  last_x = xpos;
  last_y = ypos;
  float sensitivity = 0.05;
  xoffset *= sensitivity;
  yoffset *= sensitivity;
  float& yaw = camera_.getref_yaw();
  float& pitch = camera_.getref_pitch();
  yaw += xoffset;
  pitch += yoffset;
  // TODO: use const var
}

void Editor::scrollCallback(double xoffset, double yoffset) {
  if (fov_ >= 1.0f && fov_ <= 45.0f) fov_ -= yoffset;
  if (fov_ <= 1.0f) fov_ = 1.0f;
  if (fov_ >= 45.0f) fov_ = 45.0f;
}
