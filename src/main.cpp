#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../include/Shader.h"
#include <iostream>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
GLFWwindow *gWindow = nullptr;
void framebuffer_resize_callback(GLFWwindow *window, int w, int h);
void Init();

int main() {
  const char *vertexShaderSource =
      "#version 330 core\n"
      "layout (location = 0) in vec3 aPos;\n"
    "layout (location=1) in vec3 our_color;"
      "out vec4 vertColor;"
      "void main()\n"
      "{\n"
      "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
      "vertColor = vec4(our_color, 1.0f);"
      "}\0";
  const char *fragmentShaderSource = "#version 330 core\n"
                                     "out vec4 FragColor;\n"
                                     "uniform vec4 new_color;"
                                     "in vec4 vertColor;"
                                     "void main()\n"
                                     "{\n"
                                     "   FragColor = vertColor;\n"
                                     "}\n\0";
  Init();

  int nAttrib;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nAttrib);
  printf("GPU supports %d vertex attribiutes\n", nAttrib);

  float vertices[] = {
   -0.5f, -0.5f, 0.0f, // left
    1.0f, 0.0f, 0.0f,
      0.5f,  -0.5f, 0.0f, // right
    0.0f, 1.0f, 0.0f,
      0.0f,  0.5f,  0.0f,  // top
    0.0f, 0.0f, 1.0f
  };

  unsigned int indices[] = {
      // note that we start from 0!
      0, 1, 2, // first triangle
  };

  // build and compile shaders
  //
  unsigned int vertexShader, fragmentShader, shaderProgram;

  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
  glCompileShader(vertexShader);

  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
  glCompileShader(fragmentShader);

  int sucess;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &sucess);

  if (!sucess) {

    char infoLog[512] = {'\0'};

    fprintf(stderr, "Failed to compile vertex shader\n");
    glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
    fprintf(stderr, "ERROR: %s", infoLog);
    glfwTerminate();
  }
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &sucess);

  if (!sucess) {
    char infoLog[512] = {'\0'};
    fprintf(stderr, "Failed to compile fragmentShader shader\n");
    glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
    fprintf(stderr, "ERROR: %s", infoLog);
    glfwTerminate();
  }
  Shader programShader("../shaders/vertex/basic.vert", "../shaders/fragment/basic.frag");

  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, fragmentShader);
  glAttachShader(shaderProgram, vertexShader);
  glLinkProgram(shaderProgram);
  sucess = 0;
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &sucess);
  if (!sucess) {

    char infoLog[512] = {'\0'};

    fprintf(stderr, "Failed to Linkshader\n");
    glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
    fprintf(stderr, "Error: %s\n", infoLog);
    glfwTerminate();
  }
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

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6,
                        (void *)(0));
  glVertexAttribPointer(1, 3, GL_FLOAT,GL_FALSE, sizeof(float)*6, (void*)(3*sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  // unbinding

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  // unbinding vao

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glBindVertexArray(0);

  while (!glfwWindowShouldClose(gWindow)) {
    glClearColor(0.2f, 0.5f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(VAO);

    programShader.Use();
    programShader.setFloat("u_time",glfwGetTime());
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
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
