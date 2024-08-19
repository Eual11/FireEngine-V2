#include "../include/EModel.h"
#include "../include/Shader.h"
#include "../include/Window.h"
#include "../include/stb_image.h"
#include <GLFW/glfw3.h>
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
int main() {

  auto cameraPtr = std::make_shared<Camera>(camera);
  /* INIT(); */
  Window nWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "NEW Window", false);
  nWindow.BindCamera(cameraPtr);
  // creating some shaders
  Shader shaderProgram("../shaders/vertex/diffuse_specularVert.glsl",
                       "../shaders/fragment/spotlightFrag.glsl");

  Shader lightProgram("../shaders/vertex/lightVert.glsl",
                      "../shaders/fragment/lightFrag.glsl");

  std::filesystem::path path("../models/scene.gltf");
  EModel loaded_model(std::filesystem::absolute(path).string());
  loaded_model.setScale(0.09, 0.09, 0.09);
  EModel another_mode(
      std::filesystem::absolute("../models/DamagedHelmet.gltf").string());
  another_mode.setPosition(25.0f, 3.0f, 0.0f);
  another_mode.setScale(2.0f, 2.0f, 2.0f);
  // Diffuse and Textures
  //

  // passing texture
  //
  //  camera position
  while (nWindow.isOpen()) {

    shaderProgram.Use();

    shaderProgram.setVec3("light.position", camera.Pos);
    shaderProgram.setVec3("light.direction", camera.Front);
    shaderProgram.setFloat(
        "light.cutoff",
        glm::cos(glm::radians(12.5f))); // setting the matrials

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    shaderProgram.setVec3("light.diffuse", 0.7f * lightColor);
    shaderProgram.setVec3("light.ambient", 0.6f * lightColor);

    nWindow.UpdateUniforms(shaderProgram);
    loaded_model.Draw(shaderProgram);
    another_mode.Draw(shaderProgram);
    nWindow.Update();
  }
  printf("Good Bye\n");

  return 0;
}
