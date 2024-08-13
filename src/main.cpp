#include "../include/Camera.h"
#include "../include/Model.h"
#include "../include/Shader.h"
#include "../include/Window.h"
#include "../include/stb_image.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <filesystem>
#include <glad/glad.h>
#include <glm/common.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <vector>

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;

float fYaw = 270.0f;
float fPitch = 0.0f;
float fZoom = 45.0f;
double lastX = WINDOW_WIDTH / 2.0f;
double lastY = WINDOW_HEIGHT / 2.0f;
bool firstMove = true;
glm::vec3 camPos(0.0f, 0.0f, 17.0f);
glm::vec3 camFront(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);

glm::vec3 lightPos(0.0f, 0.0f, 8.0f);

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

  auto cameraPtr = std::make_shared<Camera>(camera);
  /* INIT(); */
  stbi_set_flip_vertically_on_load(true);
  Window nWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "NEW Window", false);
  nWindow.BindCamera(cameraPtr);
  // creating some shaders
  Shader shaderProgram("../shaders/vertex/diffuse_specularVert.glsl",
                       "../shaders/fragment/spotlightFrag.glsl");

  Shader lightProgram("../shaders/vertex/lightVert.glsl",
                      "../shaders/fragment/lightFrag.glsl");

  /* Shader lighShader() */

  // VAO, VBO and EBO

  std::filesystem::path path("../models/scene.gltf");
  Model loaded_model(std::filesystem::absolute(path).string());
  loaded_model.model_transform =
      glm::scale(glm::mat4(1.0f), glm::vec3(0.09, 0.09, 0.09));
  Model another_mode(
      std::filesystem::absolute("../models/Teapot.fbx").string());
  another_mode.model_transform =
      glm::translate(glm::mat4(1.0f), glm::vec3(25.0f, 3.0f, 4));
  another_mode.model_transform =
      glm::scale(another_mode.model_transform, glm::vec3(2.0f, 2.0f, 2.0f));

  // Diffuse and Textures
  //

  unsigned int diffuseTexture, specularTexture;

  glGenTextures(1, &diffuseTexture);
  glGenTextures(1, &specularTexture);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, diffuseTexture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_NEAREST_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int w, h, nch;
  unsigned char *buffer = stbi_load("../img/container2.png", &w, &h, &nch, 0);

  if (buffer) {

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 buffer);
    glGenerateMipmap(GL_TEXTURE_2D);

  } else {
    fprintf(stderr, "failed to load texture\n");
  }

  stbi_image_free(buffer);
  buffer = nullptr;
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, specularTexture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_NEAREST_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  buffer = stbi_load("../img/container2_specular.png", &w, &h, &nch, 0);

  if (buffer) {

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 buffer);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(buffer);
    buffer = nullptr;

  } else {
    fprintf(stderr, "failed to load texture\n");
  }

  unsigned int emmisionTexture;
  glGenTextures(1, &emmisionTexture);
  buffer = nullptr;
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, emmisionTexture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_NEAREST_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  buffer = stbi_load("../img/matrix.jpg", &w, &h, &nch, 0);

  if (buffer) {

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 buffer);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(buffer);
    buffer = nullptr;

  } else {
    fprintf(stderr, "failed to load texture\n");
  }

  shaderProgram.Use();
  shaderProgram.setVec3("lightColor", glm::vec3(1.0, 1.0, 1.0));

  shaderProgram.setInt("material.texture_diffuse1", 0);
  shaderProgram.setInt("material.texture_specular1", 1);
  shaderProgram.setInt("emission", 2);
  shaderProgram.setFloat("material.shineness", 32.0f);

  shaderProgram.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.00f));

  // passing texture
  //
  //  camera position
  while (nWindow.isOpen()) {

    glm::mat4 model = glm::rotate(
        glm::mat4(1.0f), ((float)glfwGetTime() * 0.0f), glm::vec3(0, 1, 0));

    glm::mat4 view = camera.getLookAt();
    glm::mat4 project = glm::perspective(
        glm::radians(camera.Fov), nWindow.GetAspectRatio(), 0.1f, 100.0f);
    shaderProgram.Use();

    shaderProgram.setFloat("gTime", glfwGetTime());
    shaderProgram.setVec3("viewPos", camera.Pos);

    shaderProgram.setVec3("light.position", camera.Pos);
    shaderProgram.setVec3("light.direction", camera.Front);
    shaderProgram.setFloat(
        "light.cutoff",
        glm::cos(glm::radians(12.5f))); // setting the matrials
    unsigned int loc = glGetUniformLocation(shaderProgram.ID, "model");
    loc = glGetUniformLocation(shaderProgram.ID, "view");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(view));

    loc = glGetUniformLocation(shaderProgram.ID, "project");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(project));

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::vec3 lightColor(0.996, 0.753, 0.898);
    shaderProgram.setVec3("light.diffuse", 0.7f * lightColor);
    shaderProgram.setVec3("light.ambient", 0.6f * lightColor);

    lightProgram.Use();
    lightProgram.setVec3("LightColor", lightColor);

    model = glm::translate(glm::mat4(1.0f), lightPos);
    model = glm::scale(model, glm::vec3(0.2, 0.2, 0.2));
    lightProgram.setMat4("view", view);
    lightProgram.setMat4("project", project);
    lightProgram.setMat4("model", model);
    nWindow.UpdateUniforms(shaderProgram);
    nWindow.UpdateUniforms(lightProgram);
    loaded_model.Draw(shaderProgram);
    another_mode.Draw(shaderProgram);
    ;
    nWindow.Update();
  }
  printf("Good Bye\n");

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

  // she buffers my throbber, until i load
  stbi_set_flip_vertically_on_load(true);
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

  (void)window;
  (void)xpos;
  camera.ProcessScroll(ypos);
}
