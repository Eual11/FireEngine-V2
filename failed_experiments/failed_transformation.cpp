#include "../include/Shader.h"
#include "../include/stb_image.h"
#include <GLFW/glfw3.h>
#include <gl/gl.h>
#include <glad/glad.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <vector>

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;
GLFWwindow *gWindow = nullptr;

void INIT();
void framebuffer_size_callback(GLFWwindow *window, int w, int h);

int main() {

  INIT();

  std::vector<glm::vec3> cubePositions = {
      glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
      glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};
  float subeVertices[] = {
      // positions          // colors           // texture coords
      0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
      -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
  };

  unsigned int ve_indices[] = {
      // note that we start from 0!
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
  };

  float cubeVertices[] = {
      // Positions           // Colors             // Texture Coords
      // Front face (Red)
      -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, -0.5f, 0.5f, 1.0f,
      0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
      -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
      // Back face (Green)
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.5f, -0.5f, -0.5f,
      0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
      1.0f, -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
      // Left face (Blue)
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -0.5f, -0.5f, 0.5f,
      0.0f, 0.0f, 1.0f, 1.0f, 0.0f, -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
      1.0f, -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
      // Right face (Yellow)
      0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.5f, -0.5f, 0.5f, 1.0f,
      1.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
      // Top face (Cyan)
      -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f, -0.5f, 0.0f,
      1.0f, 1.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
      -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
      // Bottom face (Magenta)
      -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, -0.5f, -0.5f,
      1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 1.0f,
      1.0f, -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f};

  unsigned int indices[] = {// Front face
                            0, 1, 2, 2, 3, 0,
                            // Back face
                            4, 5, 6, 6, 7, 4,
                            // Left face
                            8, 9, 10, 10, 11, 8,
                            // Right face
                            12, 13, 14, 14, 15, 12,
                            // Top face
                            16, 17, 18, 18, 19, 16,
                            // Bottom face
                            20, 21, 22, 22, 23, 20};

  glm::mat4 rot_mat = glm::mat4(1.0f);

  rot_mat =
      glm::rotate(rot_mat, glm::radians(45.0f), glm::vec3(0.0, 0.0f, 1.0f));

  rot_mat =
      glm::rotate(rot_mat, glm::radians(45.0f), glm::vec3(0.0, 1.0f, 0.0f));
  glm::rotate(rot_mat, glm::radians(45.0f), glm::vec3(0.0, 1.0f, 0.0f));

  glm::mat4 matProj = glm::perspective(
      glm::radians(56.0f), (float)(WINDOW_WIDTH) / (float)(WINDOW_HEIGHT), 0.1f,
      100.0f);
  glm::mat4 matView = glm::translate(glm::mat4(1), glm::vec3(0, 0, -3.0));

  Shader shaderProgram("../shaders/vertex/basic2.glsl",
                       "../shaders/fragment/basic2.glsl");

  unsigned int VAO, VBO, EBO;
  glEnable(GL_DEPTH_TEST);
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices,
               GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  // vertex attrib
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8,
                        (void *)(sizeof(float) * 3));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8,
                        (void *)(sizeof(float) * 6));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
  // textures
  //
  unsigned int texture1, texture2;

  glGenTextures(1, &texture1);
  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_NEAREST);

  int w, h, nch;
  unsigned char *buffer = stbi_load("../img/img.jpg", &w, &h, &nch, 0);

  if (buffer) {
    glad_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB,
                      GL_UNSIGNED_BYTE, buffer);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    fprintf(stderr, "Failed to load image\n");
    exit(1);
  }
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture1);

  while (!glfwWindowShouldClose(gWindow)) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(VAO);
    glm::mat4 matProj = glm::perspective(
        glm::radians(56.0f), (float)(WINDOW_WIDTH) / (float)(WINDOW_HEIGHT),
        0.1f, 100.0f);
    glm::mat4 matView = glm::translate(glm::mat4(1), glm::vec3(0, 0, -3.0));

    // Copying view transform
    unsigned int loc = glGetUniformLocation(shaderProgram.ID, "view");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matView));

    // Copying projection transform
    loc = glGetUniformLocation(shaderProgram.ID, "project");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matProj));

    // copying model transform
    loc = glGetUniformLocation(shaderProgram.ID, "model");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(rot_mat));

    for (int i = 0; i < cubePositions.size(); i++) {
      shaderProgram.Use();
      rot_mat = glm::translate(rot_mat, cubePositions[i]);
      rot_mat = glm::translate(rot_mat, glm::vec3(0.0f, 0.0f, 10.0f));

      rot_mat = glm::rotate(
          glm::mat4(1.0f),
          (float)(glfwGetTime() *
                  glm::sin((float)(i * 360.0f / cubePositions.size()))),
          glm::vec3(0.0f, 1.0f, 0.0f));
      rot_mat =
          glm::rotate(rot_mat, (float)(glfwGetTime() * 1.2f * cos(i * 10)),
                      glm::vec3(1.0f, 0.0f, 0.0f));
      rot_mat = glm::rotate(rot_mat, (float)(glfwGetTime() * 0.7f),
                            glm::vec3(0.0f, 0.0f, 1.0f));
      loc = glGetUniformLocation(shaderProgram.ID, "model");
      glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(rot_mat));

      glDrawElements(GL_TRIANGLES, sizeof(cubeVertices), GL_UNSIGNED_INT, 0);
    }
    glfwPollEvents();
    glfwSwapBuffers(gWindow);
  }
  glfwTerminate();
  return 0;
}

void INIT() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  gWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Transform", nullptr,
                             nullptr);

  if (!gWindow) {

    fprintf(stderr, "Failed to Create Window\n");
    glfwTerminate();
    exit(1);
  }

  glfwMakeContextCurrent(gWindow);
  if (!(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))) {

    fprintf(stderr, "Failed to initialize GLAD\n");
    glfwTerminate();
    exit(1);
  }

  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

  glfwSetFramebufferSizeCallback(gWindow, framebuffer_size_callback);

  // enable fliping the images
  stbi_set_flip_vertically_on_load(true);
}

void framebuffer_size_callback(GLFWwindow *window, int w, int h) {
  glViewport(0, 0, w, h);
  WINDOW_WIDTH = w;
  WINDOW_HEIGHT = h;
}
