#include "../include/EModelLoader.h"
#include "../include/EWorld.h"
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
glm::vec3 camPos(0.0f, 0.0f, 17.0f);
glm::vec3 camFront(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);

glm::vec3 lightPos(0.0f, 0.0f, 8.0f);

int main() {

  Camera camera(camPos, cameraUp, fYaw, fPitch);
  auto cameraPtr = std::make_shared<Camera>(camera);
  Window nWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "NEW Window", false);
  nWindow.BindCamera(cameraPtr);
  // creating some shaders
  Shader shaderProgram("../shaders/vertex/basic.glsl",
                       "../shaders/fragment/basic.glsl");

  Shader lightProgram("../shaders/vertex/lightVert.glsl",
                      "../shaders/fragment/lightFrag.glsl");
  EWorld zaWardu;
  // TODO: tidy this up, it is disguting
  std::filesystem::path path("../models/scene.gltf");

  EModelLoader loader;
  auto cube = loader.loadModel(
      std::filesystem::absolute("../models/cube.obj").string());

  cube->setPosition(6, 0, 0);
  cube->setRotation(0.0f, 45.0f, 0);
  auto another_mode = loader.loadModel(
      std::filesystem::absolute("../models/cube.obj").string());

  another_mode->setPosition(3, 0, 0);
  another_mode->add(cube);
  auto pnt = std::make_shared<PointLight>(
      glm::vec3(1.0f, 0.3f, 0.1f), glm::vec3(1.0f, 1.0f, 1.0f),
      glm::vec3(5, 15, 7), 3.0f, 0.045f, 0.0075f);
  auto pnt2 = std::make_shared<PointLight>(glm::vec3(0.0f, 1.0f, 0.1f),
                                           glm::vec3(1.0f, 0.1f, 1.0f),
                                           glm::vec3(-7, 3, 7), 3.0f);
  auto pnt3 = std::make_shared<PointLight>(glm::vec3(0.2f, 0.1f, 1.0f),
                                           glm::vec3(1.0f, 1.0f, 1.0f),
                                           glm::vec3(7, 5, 12), 3.0f);
  AmbientLight amb(glm::vec3(1.0f, 1.0f, 1.0f), 0.6);
  SpotLight spt({1.0f, 1.0f, 1.0f}, {1.0f, 1.0, 1.0}, {0.0f, 20.0f, 3.0f},
                {0.0f, -1.0f, 0.f}, 1.0f, glm::cos(glm::radians(30.0f)),
                glm::cos(glm::radians(50.0f)));
  zaWardu.AddObject(cube, shaderProgram);
  zaWardu.AddObject(another_mode, shaderProgram);

  zaWardu.AddLight(std::make_shared<SpotLight>(spt));
  zaWardu.AddLight(pnt);
  zaWardu.AddLight(pnt2);
  zaWardu.AddLight(pnt3);
  zaWardu.AddLight(std::make_shared<AmbientLight>(amb));
  float spiralRadius = 5.0f;
  float spiralHeight = 10.0f;
  float spiralSpeed = 1.0f;

  while (nWindow.isOpen()) {
    float angle = glfwGetTime() * spiralSpeed; // Angle based on time and speed

    // Calculate x and z positions using sine and cosine
    float x = spiralRadius * cos(angle);
    float z = spiralRadius * sin(angle);
    float y = spiralHeight * (sin(angle / 2.0f) + 1.0f) /
              2.0f; // Adjust height to create the spiral effect
    pnt->setPosition(x, y, z);
    pnt2->setPosition(x + 2.0f, y, z + 2.0f); // Offset for different lights
    pnt3->setPosition(x + 4.0f, y, z + 4.0f);

    shaderProgram.setVec3("light.position", camera.Position);
    shaderProgram.setVec3("light.direction", camera.Front);
    shaderProgram.setFloat(
        "light.cutoff",
        glm::cos(glm::radians(12.5f))); // setting the matrials

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    nWindow.UpdateUniforms(shaderProgram);
    zaWardu.Render();
    nWindow.Update();
  }
  printf("Good Bye\n");

  return 0;
}
