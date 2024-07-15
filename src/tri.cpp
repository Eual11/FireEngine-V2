#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

GLFWwindow *gWindow = nullptr;

void framebuffer_resize_callback(GLFWwindow *window, int w, int h);
void HandleInput(GLFWwindow *window);
void Init();

const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location =1 ) in vec3 aPos;\n"
    "void main(){"
    "    gl_Position = vec4(aPos.x, aPos.y, aPos.z,1.0f);\n"
    "}";
const char *fragmentShaderSource = "#version 330 core\n"
                                   "void main(){\n"
                                   "gl_FragColor = vec4(1.0, 0.5, 0,1.0f);}";

int main(void) {
  Init();

  float triVertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f,
                         0.0f,  0.0f,  0.5f, 0.0f};

  unsigned int VBO;
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glGenBuffers(1, &VBO);

  // binding VBO to GL_ARRAY_BUFFER mode, now any subsequent call to
  // GL_ARRAY_BUFFER will manipulate and alter our VBO
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // allocating data on the gpu's memory and copy our triVertices data managed
  // by our VBO
  glBufferData(GL_ARRAY_BUFFER, sizeof(triVertices), triVertices,
               GL_STATIC_DRAW);

  unsigned int vertexShader = 0;
  unsigned int fragmentShader = 0;

  // glCreateShader shader creates a shader object behind the scene and returns
  // the ID of it
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

  // replaces the shader source for the given shader, every time its compield
  // the source is taken from here
  glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
  glCompileShader(vertexShader);   // compiling the shader
  glCompileShader(fragmentShader); // compiling the fragment shader
  //
  int sucess = 0;

  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &sucess);
  if (!sucess) {
    fprintf(stderr, "Failed to compile shader\n");
    char infoLog[512];
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    fprintf(stderr, "COMPILATION FAILED:: %s", infoLog);
    glfwTerminate();
    return -1;
  }

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &sucess);
  if (!sucess) {
    fprintf(stderr, "Failed to compile shader\n");
    char infoLog[512];
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    fprintf(stderr, "COMPILATION FAILED:: %s", infoLog);
    glfwTerminate();
    return -1;
  }
  // creating the final shader program
  unsigned int shaderProgram = 0;

  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);

  glLinkProgram(shaderProgram);
  sucess = 0;
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &sucess);
  if (!sucess) {
    fprintf(stderr, "Failed to link program\n");
    glfwTerminate();
    return -1;
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
  // making the window context current for this thread
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  // setting the window resize callback
  //
  glfwSetFramebufferSizeCallback(gWindow, framebuffer_resize_callback);

  while (!glfwWindowShouldClose(gWindow)) {
    HandleInput(gWindow);

    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 3);
    glfwPollEvents();
    glfwSwapBuffers(gWindow);
  }

  glfwTerminate();
  return 0;
}
void framebuffer_resize_callback(GLFWwindow *window, int w, int h) {
  glViewport(0, 0, w, h);
}
void HandleInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(gWindow, GLFW_TRUE);
  }
}

void Init() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // creating the window

  gWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
                             "Please God... Make this work", nullptr, nullptr);

  if (!gWindow) {
    fprintf(stderr, "Couldn't Create Window\n");
    glfwTerminate();
    exit(1);
  }
  glfwMakeContextCurrent(gWindow);

  // loading gladloder

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "Couldn't load glad\n");
    exit(1);
    glfwTerminate();
  }
}
