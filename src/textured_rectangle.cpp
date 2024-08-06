#include "../include/Shader.h"
#include "../include/stb_image.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
GLFWwindow *gWindow = nullptr;
void framebuffer_resize_callback(GLFWwindow *window, int w, int h);
void Init();

int main() {
  Init();

  int nAttrib;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nAttrib);
  printf("GPU supports %d vertex attribiutes\n", nAttrib);

  float vertices[] = {
      // positions          // colors           // texture coords
      0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
      1.0f, // top right
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
      0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, // bottom left
      -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
      1.0f // top left   };
  };
  unsigned int indices[] = {
      // note that we start from 0!
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
  };
  // build and compile shaders
  //

  Shader programShader("../shaders/vertex/basic.vert",
                       "../shaders/fragment/basic.frag");

  unsigned int VAO, VBO, EBO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8,
                        (void *)(0));
  // unbinding

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  // unbinding vao

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glBindVertexArray(0);

  // Loading texture

  while (!glfwWindowShouldClose(gWindow)) {
    glClearColor(0.0, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(VAO);

    programShader.Use();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glfwPollEvents();
    glfwSwapBuffers(gWindow);
  }
  glfwTerminate();
  return 0;
}
void framebuffer_resize_callback(GLFWwindow *window, int w, int h) {
  glViewport(0, 0, w, h);
}

void Init() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  gWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Rectangle", nullptr,
                             nullptr);
  if (!gWindow) {
    fprintf(stderr, "Couldn't create GLFW Window\n");
    exit(1);
    glfwTerminate();
  }

  glfwMakeContextCurrent(gWindow);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "Couldn't Intialize Glad\n");
    exit(1);
    glfwTerminate();
  }
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  glfwSetFramebufferSizeCallback(gWindow, framebuffer_resize_callback);
}
