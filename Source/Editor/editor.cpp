#include "editor.h"
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "../../ThirdLib/stb_image.h"
#endif

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
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  glEnable(GL_DEPTH_TEST);
  // disable mouse cursor and capture it
  glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window_, glmouseCallbackWrapper);
  glfwSetScrollCallback(window_, glscrollCallbackWrapper);
  return 0;
}

void Editor::initVertexArrays() {
  // cube VAO
  glGenVertexArrays(1, &cube_VAO_);
  glBindVertexArray(cube_VAO_);
  //belong to cube_VAO_
  {
    glGenBuffers(1, &VBO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(float),
                 vertices_.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &EBO_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices_.size() * sizeof(unsigned int), indices_.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT),
                          reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    // texture coordinate x,y, 2 * 4 bytes
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT),
                          reinterpret_cast<void*>(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);
  }
  // light VAO
  glGenVertexArrays(1, &light_VAO_);
  glBindVertexArray(light_VAO_);
  //belong to light_VAO_
  {
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT),
                          reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    // texture coordinate x,y, 2 * 4 bytes
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT),
                          reinterpret_cast<void*>(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);
  }
}

void Editor::loadTextures() {
  glGenTextures(1, &texture_);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  int width, height, nrChannels;
  unsigned char* data = stbi_load("Resource/Texture/container.jpg", &width,
                                  &height, &nrChannels, 0);
  if (data) {
    std::cout << "Texture width x height = " << width << " x " << height
              << std::endl;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);
  shader_.use();
  shader_.setUniformOneValue<GLint>("texture1", 0);
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
  light_shader_ = Shader("Source/ShaderSource/light.vert",
                         "Source/ShaderSource/light_shader.frag");
  shader_.use();
  shader_.setUniformVec3f("light_color", glm::vec3(1, 0, 0));
}

Editor::Editor(int screen_width, int screen_height)
    : screen_height_(screen_height), screen_width_(screen_width) {
  EBO_ = 0;
  VBO_ = 0;
  cube_VAO_ = 0;
  light_VAO_ = 0;
  texture_ = 0;
  window_ = 0;
}

int Editor::run() {
  editor = this->shared_from_this();
  int exit_code = initEnvironment();
  if (exit_code) {
    return exit_code;
  }
  initVertexArrays();
  loadTextures();
  initCamera();
  initShader();
  float last_time = 0;
  float delta_time = 0;
  while (!glfwWindowShouldClose(window_)) {
    // render process
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_);
    float cur_time = glfwGetTime();
    delta_time = cur_time - last_time;
    last_time = cur_time;
    camera_.set_move_speed(delta_time * 5);
    /*TRANSFORM BEGIN*/
    //camera_.moveByEulerianAngles();
    auto m_view = camera_.getLookAtMat();
    auto m_projection = Transform::projectionTrans(
        fov_, static_cast<float>(screen_width_) / screen_height_, 0.1f, 100.0f);
    auto setMatrixUniform = [](Shader& shader, const glm::mat4& matrix,
                               const std::string& matrix_name,
                               GLint boolean = GL_FALSE) -> void {
      auto matrix_location =
          glGetUniformLocation(shader.get_ID(), matrix_name.c_str());
      glUniformMatrix4fv(matrix_location, 1, boolean, glm::value_ptr(matrix));
    };
    /*TRANSFORM END*/
    shader_.use();
    setMatrixUniform(shader_, m_view, "m_view", GL_TRUE);
    setMatrixUniform(shader_, m_projection, "m_projection", GL_TRUE);
    glBindVertexArray(cube_VAO_);
    for (int i = 0; i < 10; i++) {
      auto m_model = Transform::modelTrans(
          (i + 1) * 20.0f, glm::vec3(1.0, 0.3, 0.5), 1, cube_positions_[i]);
      setMatrixUniform(shader_, m_model, "m_model");
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    light_shader_.use();
    glBindVertexArray(light_VAO_);
    setMatrixUniform(light_shader_, m_view, "m_view", GL_TRUE);
    setMatrixUniform(light_shader_, m_projection, "m_projection", GL_TRUE);
    auto m_model =
        Transform::modelTrans(45, glm::vec3(1, 1, 0), 0.2, light_pos_);
    setMatrixUniform(light_shader_, m_model, "m_model");
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    processKeyBoardInput(window_);
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
  float pitch = camera_.getref_pitch();
  yaw += xoffset;
  camera_.getref_pitch() += yoffset;
  // TODO: use const var
  if (pitch > 89.0f) {
    pitch = 89;
  }
  if (pitch < -89.0f) {
    pitch = -89;
  }
}

void Editor::scrollCallback(double xoffset, double yoffset) {
  if (fov_ >= 1.0f && fov_ <= 45.0f) fov_ -= yoffset;
  if (fov_ <= 1.0f) fov_ = 1.0f;
  if (fov_ >= 45.0f) fov_ = 45.0f;
}
