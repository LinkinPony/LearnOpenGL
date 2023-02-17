#include <iostream>

// GLAD, include it before GLFW
#include <glad/glad.h>
// GLFW
#include <GLFW/glfw3.h>

#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "Source/Shader/shader.h"
#include "Source/Shader/transform.h"
#include "ThirdLib/stb_image.h"
#include "Source/camera.h"

const int screen_width = 800;
const int screen_height = 600;

float vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
    0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

    -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
    0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
    0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

    -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

unsigned int indices[] = {
    // 注意索引从0开始!
    // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
    // 这样可以由下标代表顶点组合成矩形
    0, 1, 3,  // 第一个三角形
    1, 2, 3};
glm::vec3 cubePositions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
    glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};
// call back function
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
  return;
}
void process_input(GLFWwindow* window,Camera & camera) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera.moveByDirection(Camera::MoveDirection::kForward);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera.moveByDirection(Camera::MoveDirection::kLeft);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera.moveByDirection(Camera::MoveDirection::kBack);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera.moveByDirection(Camera::MoveDirection::kRight);
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    camera.moveByDirection(Camera::MoveDirection::kUp);
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    camera.moveByDirection(Camera::MoveDirection::kDown);
  }
}
GLuint configueVBO() {
  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  return VBO;
}
GLuint configueEBO() {
  GLuint EBO;
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);
  return EBO;
}
GLuint configueVAO() {
  GLuint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // generate VBO and bind it to current VAO
  auto VBO = configueVBO();
  // EBO too
  auto EBO = configueEBO();
  // coordinate x,y,z. 3 * 4 bytes
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT),
                        reinterpret_cast<void*>(0));
  glEnableVertexAttribArray(0);
  // texture coordinate x,y, 2 * 4 bytes
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT),
                        reinterpret_cast<void*>(3 * sizeof(GL_FLOAT)));
  glEnableVertexAttribArray(1);

  return VAO;
}
int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  GLFWwindow* window = glfwCreateWindow(screen_width, screen_height,
                                        "Demo window", nullptr, nullptr);
  if (window == nullptr) {
    std::cout << "Create window failed." << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  GLuint VAO = configueVAO();
  Shader shader("Source/ShaderSource/vertex_shader.vert",
                "Source/ShaderSource/fragment_shader.frag");

  /* TEXTURE BEGIN */
  GLuint texture;
  glGenTextures(1, &texture);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
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
  shader.use();
  shader.setUniformValue<GLint>("texture1", 0);
  /* TEXTURE END */
  auto camera_position = glm::vec3(0,0,3);
  auto camera_direction = glm::vec3(0,0,-1);
  auto up_direction = glm::vec3(0,1,0);
  Camera camera(camera_position,camera_direction,up_direction);
  camera.set_move_speed(0.05);
  float last_time = 0;
  float delta_time = 0;
  glEnable(GL_DEPTH_TEST);
  while (!glfwWindowShouldClose(window)) {
    // render process
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    float cur_time = glfwGetTime();
    delta_time = cur_time - last_time;
    last_time = cur_time;
    camera.set_move_speed(delta_time * 5);
    /*TRANSFORM BEGIN*/
    float radius = 10.0f;
    float camX = sin(glfwGetTime()) * radius;
    float camZ = cos(glfwGetTime()) * radius;
    //camera.set_camera_position(glm::vec3(camX, 0.0, camZ));
    //auto identity = glm::mat4(1.0f);
    auto m_view = camera.getLookAtMat();
    //m_view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0),
    //                   glm::vec3(0.0, 1.0, 0.0)); 
    auto m_projection = glm::perspective(
        glm::radians(45.0f), static_cast<float>(screen_width) / screen_height,
        0.1f, 100.0f);
    auto setMatrixUniform = [&shader](const glm::mat4& matrix,
                                      const std::string& matrix_name,GLint boolean = GL_FALSE) -> void {
      auto matrix_location =
          glGetUniformLocation(shader.get_ID(), matrix_name.c_str());
      glUniformMatrix4fv(matrix_location, 1, boolean, glm::value_ptr(matrix));
    };
    setMatrixUniform(m_view, "m_view",GL_FALSE);
    setMatrixUniform(m_projection, "m_projection");
    /*TRANSFORM END*/
    shader.use();
    glBindVertexArray(VAO);
    for (int i = 0; i < 10; i++) {
      auto m_model =
          Transform::modelTrans((i + 1) * 20.0f,
                                glm::vec3(1.0, 0.3, 0.5), 1, cubePositions[i]);
      setMatrixUniform(m_model, "m_model");
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    glBindVertexArray(0);
    process_input(window,camera);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}