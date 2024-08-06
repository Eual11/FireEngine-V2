#include <glad/glad.h>
#include <iostream>
#include <GLFW/glfw3.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEGHT 600


//global window
GLFWwindow* gWindow = nullptr;
void framebuffer_resize_callback(GLFWwindow* window, int w, int h);
void Init();
const char* vertexShaderSrc = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
}
)";

const char* fragmentShaderSrc = R"(
#version 330 core
out vec4 FragColor;
void main()
{
    FragColor = vec4(0.2, 0.1, 0.6, 1.0f);
}
)";


int main()
{

  Init();

  float verticies[] = {
    //tri1
  0.0f, 0.0f,0.0f,
 -0.5f,0.5f, 0.0f, 
 -0.5f, 0.0f, 0.0f,

    //tri2
  0.0f, 0.0f,0.0f,
  0.0f, 0.5f, 0.0f,
  0.5f, 0.0f, 0.0f
  };

  float tri1Verticies[] =
    {
  0.0f, 0.0f,0.0f,
 -0.5f,0.5f, 0.0f, 
 -0.5f, 0.0f, 0.0f,

    };


  float tri2Verticies[]
  {
  0.0f, 0.0f,0.0f,
  0.0f, 0.5f, 0.0f,
  0.5f, 0.0f, 0.0f

  };


  //creating shaders
  unsigned int vertexShader, fragmentShader;

  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSrc, nullptr);
  glShaderSource(fragmentShader, 1, &fragmentShaderSrc, nullptr);

  //compiling both shaders
  glCompileShader(vertexShader);
  glCompileShader(fragmentShader);

  //checking compilation status 
  int sucess = 0;

  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &sucess);
  if(!sucess)
  {
    //error reporting
    char infolog[512];
    fprintf(stderr, "Failed to compile vertex shader\n");
    glGetShaderInfoLog(vertexShader, 512, nullptr, infolog);
    fprintf(stderr, "%s\n", infolog);
  }
  
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &sucess);
  if(!sucess)
  {
    //error reporting
    char infolog[512];
    fprintf(stderr, "Failed to compile vertex shader\n");
    glGetShaderInfoLog(fragmentShader, 512, nullptr, infolog);
    fprintf(stderr, "%s\n", infolog);
  }

  //shader program

  unsigned int shaderProgram;

  shaderProgram = glCreateProgram();
  //attaching vertex and fragment shaders
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);


  //linking shader program
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &sucess);
  if(!sucess)
  {
    char infolog[512];
    fprintf(stderr, "Failed to Link shader program\n");
    glGetProgramInfoLog(shaderProgram, 512, nullptr, infolog);
    fprintf(stderr, "%s\n", infolog);
  }

  //VAO and VBO time
  unsigned int VAOs[2];
  unsigned int VBOs[2];




  //creating the vao and binding it
  glGenVertexArrays(2, VAOs);
  //binding the first array
  glBindVertexArray(VAOs[0]);
  glGenBuffers(2, VBOs);
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(tri1Verticies),tri1Verticies, GL_STATIC_DRAW);

  //setting vertex atrtrib 

  glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE,sizeof(float)*3, (void*)NULL);
  glBindVertexArray(0);
  glEnableVertexAttribArray(VAOs[1]);
  //unbinding vao and vbo
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(tri2Verticies), tri2Verticies, GL_STATIC_DRAW);
  glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE,sizeof(float)*3, (void*)0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);
  glBindVertexArray(0);


//changing polygon mode


  while(!glfwWindowShouldClose(gWindow))
  {
    glClearColor(1.0f,1.0f,1.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(VAOs[0]);
    glUseProgram(shaderProgram);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glfwPollEvents();
    glfwSwapBuffers(gWindow);
  }


  //initalization 
  return 0;
}
void Init()
{
    glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  gWindow = glfwCreateWindow(WINDOW_WIDTH,WINDOW_HEGHT, "Two Triangles", nullptr, nullptr);

  if(!gWindow)
  {
    fprintf(stderr, "Error: Couldn't Create Widnow\n");
    exit(1);
    glfwTerminate();
  }
  //making window current
  glfwMakeContextCurrent(gWindow);

  //initaliztng glad
  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    fprintf(stderr, "Error: Couldn't Create Window\n");
    exit(1);
    glfwTerminate();
  }

  //setting resize callback
  glViewport(0,0,WINDOW_WIDTH, WINDOW_HEGHT);
  glfwSetFramebufferSizeCallback(gWindow, framebuffer_resize_callback);


}
void framebuffer_resize_callback(GLFWwindow* window, int w, int h)
{
  glViewport(0,0,w, h);
}
