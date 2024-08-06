#include <glad/glad.h>
#include "../include/Shader.h"
#include "../include/stb_image.h"
#include <iostream>
#include <GLFW/glfw3.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
GLFWwindow* gWindow = nullptr;

void INIT();
void framebuffer_resize(GLFWwindow* window, int w, int h);

int main()
{
  INIT();

  
float vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};

unsigned int indices[] = {
      // note that we start from 0!
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
  };

  Shader programShader("../shaders/vertex/basic2.glsl", "../shaders/fragment/basic2.glsl");


  unsigned int  VAO, VBO, EBO;

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*) (sizeof(float)*3));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*) (sizeof(float)*6));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);


  //unbinding VAO
  glBindVertexArray(0);


  unsigned int texture1, texture2;


  glGenTextures(1, &texture1);
  glGenTextures(1, &texture2);


  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  //loading image using stbimage


  int width, height, nChannels;
  unsigned char * buffer = stbi_load("../img/img.jpg",&width, &height, &nChannels,0);

  if(buffer)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);
    glGenerateMipmap(0);
  }
  else {
    fprintf(stderr, "Failed to Laod image\n");
    glfwTerminate();
    return -1;
  }
  stbi_image_free(buffer); //freeing iamge 1
  
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texture2);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

  buffer = stbi_load("../img/overlay.png", &width, &height, &nChannels, 0);
  if(buffer)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    glGenerateMipmap(0);
  }
  programShader.Use();
  programShader.setInt("texture1", 0);
  programShader.setInt("texture1", 1);
 

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  while(!glfwWindowShouldClose(gWindow))
  {

    glClearColor(0.0f,0.0f,0.0f,1.0f);
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

void INIT()
{
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  gWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Texture Overlay", nullptr, nullptr);

  if(!gWindow)
  {
    fprintf(stderr, "Error Creating Window\n");
    exit(1);
  }

  glfwMakeContextCurrent(gWindow);

  if(!(gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress))))
  {
    fprintf(stderr, "Failed to Intialize GLAD\n");
    exit(1);
  }

  glViewport(0,0, WINDOW_WIDTH, WINDOW_HEIGHT);
  glfwSetFramebufferSizeCallback(gWindow, framebuffer_resize);

}


void framebuffer_resize(GLFWwindow* window, int w, int h)
{
  glViewport(0,0, w, h);
}
