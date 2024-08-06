#include "../include/Camera.h"
#include "../include/Shader.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <complex>
#include <glad/glad.h>
#include <glm/common.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;

float fYaw = 270.0f;
float fPitch = 0.0f;
float fZoom = 45.0f;
double lastX = WINDOW_WIDTH / 2.0f;
double lastY = WINDOW_HEIGHT / 2.0f;
bool firstMove = true;
glm::vec3 camPos(0.0f, 0.0f, 7.0f);
glm::vec3 camFront(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);

glm::vec3 lightPos(0.0, -0.5, 7.0);

Camera camera(camPos, cameraUp, fYaw, fPitch);
float lastFrame = 0.0;
float deltaTime = 0.0;
GLFWwindow *gWindow = nullptr;
void INIT();

void framebuffer_resize(GLFWwindow *window, int w, int h);
void mouse_callback(GLFWwindow *window, double xPos, double yPos);
void scroll_callback(GLFWwindow *gWindow, double xPos, double yPos);

void HandleInput();
int main() {

  INIT();
  std::vector<glm::vec3> cubePositions = {
      glm::vec3(0.0f, 0.0f, 0.0f),
  };

  // Cube vertex data
  float cubeVertices[] = {
      // Positions          // Normals           // Texture Coords
      -1.0f, -1.0f, -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f,
      1.0f,  -1.0f, -1.0f, 0.0f,  0.0f,  -1.0f, 1.0f, 0.0f,
      1.0f,  1.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f,
      -1.0f, 1.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f, 1.0f,

      -1.0f, -1.0f, 1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
      1.0f,  -1.0f, 1.0f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
      1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
      -1.0f, 1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

      -1.0f, 1.0f,  1.0f,  -1.0f, 0.0f,  0.0f,  1.0f, 0.0f,
      -1.0f, 1.0f,  -1.0f, -1.0f, 0.0f,  0.0f,  1.0f, 1.0f,
      -1.0f, -1.0f, -1.0f, -1.0f, 0.0f,  0.0f,  0.0f, 1.0f,
      -1.0f, -1.0f, 1.0f,  -1.0f, 0.0f,  0.0f,  0.0f, 0.0f,

      1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
      1.0f,  1.0f,  -1.0f, 1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
      1.0f,  -1.0f, -1.0f, 1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
      1.0f,  -1.0f, 1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,

      -1.0f, -1.0f, -1.0f, 0.0f,  -1.0f, 0.0f,  0.0f, 1.0f,
      1.0f,  -1.0f, -1.0f, 0.0f,  -1.0f, 0.0f,  1.0f, 1.0f,
      1.0f,  -1.0f, 1.0f,  0.0f,  -1.0f, 0.0f,  1.0f, 0.0f,
      -1.0f, -1.0f, 1.0f,  0.0f,  -1.0f, 0.0f,  0.0f, 0.0f,

      -1.0f, 1.0f,  -1.0f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
      1.0f,  1.0f,  -1.0f, 0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
      1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
      -1.0f, 1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f};

  unsigned int indices[] = {
      0,  1,  2,  2,  3,  0,  // Back face
      4,  5,  6,  6,  7,  4,  // Front face
      8,  9,  10, 10, 11, 8,  // Left face
      12, 13, 14, 14, 15, 12, // Right face
      16, 17, 18, 18, 19, 16, // Bottom face
      20, 21, 22, 22, 23, 20  // Top face
  };
  // creating some shaders
  Shader shaderProgram("../shaders/vertex/cubeGouradShading.glsl",
                       "../shaders/fragment/cubeGouradShading.glsl");

  Shader lightProgram("../shaders/vertex/lightVert.glsl",
                      "../shaders/fragment/lightFrag.glsl");

  /* Shader lighShader() */

  // VAO, VBO and EBO

  unsigned int VAO, lightVAO, VBO, EBO;

  glGenVertexArrays(1, &VAO);
  glGenVertexArrays(1, &lightVAO);
  glBindVertexArray(VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices,
               GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8,
                        (void *)(3 * sizeof(float)));

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8,
                        (void *)(6 * sizeof(float)));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  /* glEnableVertexAttribArray(2); */

  glBindVertexArray(0);
  glBindVertexArray(lightVAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)0);
  glEnableVertexAttribArray(0);

  shaderProgram.Use();
  shaderProgram.setVec3("lightColor", glm::vec3(1.0, 1.0, 1.0));
  shaderProgram.setVec3("objectColor", glm::vec3(0.7, 0.2, 0.5));
  shaderProgram.setVec3("viewPos", camera.Pos);

  // camera position
  while (!glfwWindowShouldClose(gWindow)) {

    HandleInput();
    glm::mat4 model = glm::rotate(
        glm::mat4(1.0f), ((float)glfwGetTime() * 0.0f), glm::vec3(0, 1, 0));

    glm::mat4 view = camera.getLookAt();
    glm::mat4 project = glm::perspective(
        glm::radians(camera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT,
        0.1f, 100.0f);

    shaderProgram.Use();
    shaderProgram.setVec3("viewPos", camera.Pos);

    shaderProgram.setVec3("lightPos", lightPos);
    unsigned int loc = glGetUniformLocation(shaderProgram.ID, "model");
    loc = glGetUniformLocation(shaderProgram.ID, "view");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(view));

    loc = glGetUniformLocation(shaderProgram.ID, "project");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(project));

    glBindVertexArray(VAO);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (size_t i = 0; i < cubePositions.size(); i++) {

      glm::mat4 rot_mat = glm::translate(glm::mat4(1.0f), cubePositions[i]);
      /* rot_mat = glm::rotate( */
      /*     rot_mat, */
      /*     (float)(glfwGetTime() * */
      /*             glm::sin((float)(2 * i * 360.0f / cubePositions.size()))),
       */
      /*     glm::vec3(0.0f, 1.0f, 0.0f)); */
      /* rot_mat = */
      /*     glm::rotate(rot_mat, (float)(glfwGetTime() * 2.2f * cos(i * 10)),
       */
      /*                 glm::vec3(1.0f, 0.0f, 0.0f)); */
      /* rot_mat = glm::rotate(rot_mat, (float)(glfwGetTime() * 1.7f), */
      /*                       glm::vec3(0.0f, 0.0f, 1.0f)); */
      loc = glGetUniformLocation(shaderProgram.ID, "model");

      glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(rot_mat));

      glDrawElements(GL_TRIANGLES, sizeof(cubeVertices), GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(lightVAO);
    lightProgram.Use();

    model = glm::translate(glm::mat4(1.0f), lightPos);
    model = glm::scale(model, glm::vec3(0.2, 0.2, 0.2));
    lightProgram.setMat4("view", view);
    lightProgram.setMat4("project", project);
    lightProgram.setMat4("model", model);

    glDrawElements(GL_TRIANGLES, sizeof(cubeVertices), GL_UNSIGNED_INT, 0);

    deltaTime = glfwGetTime() - lastFrame;
    lastFrame = glfwGetTime();
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
    fprintf(stderr, "Failed to create Window");
    exit(1);
  }

  glfwMakeContextCurrent(gWindow);
  glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  if (!(gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress)))) {
    fprintf(stderr, "Failed to initalize GLAD");
    exit(1);
  }

  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  // enabling global depth testing
  glEnable(GL_DEPTH_TEST);

  // resize callbacks

  glfwSetFramebufferSizeCallback(gWindow, framebuffer_resize);
  glfwSetCursorPosCallback(gWindow, mouse_callback);
  glfwSetScrollCallback(gWindow, scroll_callback);
}

void framebuffer_resize(GLFWwindow *window, int w, int h) {
  glViewport(0, 0, w, h);
  WINDOW_WIDTH = w;
  WINDOW_HEIGHT = h;
}

void HandleInput() {

  CameraMovement movement = CameraMovement::STATIC;
  if (glfwGetKey(gWindow, GLFW_KEY_W) == GLFW_PRESS) {
    movement = CameraMovement::FORWARD;
  }
  if (glfwGetKey(gWindow, GLFW_KEY_S) == GLFW_PRESS) {
    movement = CameraMovement::BACKWARD;
  }
  if (glfwGetKey(gWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {

    glfwSetWindowShouldClose(gWindow, true);
  }
  if (glfwGetKey(gWindow, GLFW_KEY_A) == GLFW_PRESS) {
    movement = CameraMovement::LEFT;
  }
  if (glfwGetKey(gWindow, GLFW_KEY_D) == GLFW_PRESS) {
    movement = CameraMovement::RIGHT;
  }
  camera.ProcessInput(movement, deltaTime);
}
void mouse_callback(GLFWwindow *window, double xPos, double yPos) {

  float deltaX = lastX - xPos;
  float deltaY = lastY - yPos;

  if (firstMove) {
    deltaX = 0.0;
    deltaY = 0.0;

    firstMove = false;
  }

  else {
    camera.ProcessMouseMovement(deltaX, deltaY, true);
  }
  lastX = xPos;
  lastY = yPos;
}

void scroll_callback(GLFWwindow *window, double xpos, double ypos) {

  camera.ProcessScroll(ypos);
}
