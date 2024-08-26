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
  EModel loaded_model(std::filesystem::absolute(path).string());
  loaded_model.setScale(0.09, 0.09, 0.09);
  EModel another_mode(
      std::filesystem::absolute("../models/Teapot.fbx").string());
  another_mode.setPosition(25.0f, 3.0f, 0.0f);
  another_mode.setScale(2.0f, 2.0f, 2.0f);

  AmbientLight amb(glm::vec3(1.0f, 1.0f, 1.0f), 0.5);
  DirectionalLight dir({1.0, 1.0, 1.0}, {1.0f, 1.0f, 1.0f}, {0, -1, 0});
  PointLight pnt({1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0, 0, 9});
  PointLight pnt2({0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0, 0, 30});
  zaWardu.AddObject(std::make_shared<EModel>(loaded_model), shaderProgram);
  zaWardu.AddObject(std::make_shared<EModel>(another_mode), shaderProgram);
  zaWardu.AddLight(std::make_shared<PointLight>(pnt));
  zaWardu.AddLight(std::make_shared<PointLight>(pnt2));
  /* zaWardu.AddLight(std::make_shared<AmbientLight>(amb)); */
  /* zaWardu.AddLight(std::make_shared<DirectionalLight>(dir)); */

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
    zaWardu.Render();
    nWindow.Update();
  }
  printf("Good Bye\n");

  return 0;
}
