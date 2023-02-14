#include <iostream>
// GLAD, include it before GLFW
#include <glad/glad.h> 
// GLFW
#include <GLFW/glfw3.h>

#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "ThirdLib/stb_image.h"
#include "shader.h"

float vertices[] = {
    // λ��              // ��ɫ
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // ����
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // ����
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // ����
};

unsigned int indices[] = {
    // ע��������0��ʼ! 
    // ����������(0,1,2,3)���Ƕ�������vertices���±꣬
    // �����������±��������ϳɾ���

    0, 1, 2, // ��һ��������
};
//call back function
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    return;
}
void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window,true);
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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    return EBO;
}
GLuint configueVAO() {
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
   
    //generate VBO and bind it to current VAO
    auto VBO = configueVBO();
    //EBO too
    auto EBO = configueEBO();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), reinterpret_cast<void*>(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);

    return VAO;
}
int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Demo window", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Create window failed." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    GLuint VAO = configueVAO();
    Shader shader("ShaderSource/vertex_shader.vert", "ShaderSource/fragment_shader.frag");
    while (!glfwWindowShouldClose(window)) {
        //render process
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //change color by time
        shader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        process_input(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
	return 0;
}