#include "../include/EBoxGeometry.h"
#include "../include/EModelLoader.h"
#include "../include/ERenderer.h"
#include "../include/EWorld.h"
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
#include <glm/ext/scalar_constants.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;

float fYaw = 270.0f;
float fPitch = 0.0f;
float fZoom = 45.0f;
glm::vec3 camPos(0.0f, 40.0f, 100.0f);
glm::vec3 camFront(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);

glm::vec3 lightPos(0.0f, 0.0f, 8.0f);

// TODO: somehow the child mesh's frag shaders is being executed
int main() {
  Camera camera(camPos, cameraUp, fYaw, fPitch);
  auto cameraPtr = std::make_shared<Camera>(camera);
  Window nWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Firev2", false);
  nWindow.BindCamera(cameraPtr);

  auto zaWardu = std::make_shared<EWorld>(&nWindow);
  ERenderer rend(&nWindow);

  // TODO: tidy this up, it is disguting
  UniformMap uniforms = {{"time", 0.0f}, {"uAmp", 2.0f}};

  auto tex = ETexture::load(
      "../models/textures/3DLABbg_UV_Map_Checker_01_2048x2048.jpg");
  uniforms["tex"] = tex;
  EModelLoader loader;
  auto mat = std::make_shared<ShaderMaterial>("../shaders/vertex/basic.glsl",
                                              "../shaders/fragment/basic.glsl",
                                              uniforms);

  unsigned int amount = 10000;
  std::vector<glm::mat4> modelMatrices;
  modelMatrices.resize(amount);
  srand(glfwGetTime()); // initialize random seed
  float radius = 50.0;
  float offset = 40.5f;
  for (unsigned int i = 0; i < amount; i++) {
    glm::mat4 model = glm::mat4(1.0f);

    // translate
    //
    float angle = (float)i / (float)amount * 360.0f;
    float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
    float x = sin(angle) * radius + displacement;
    displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
    float y = displacement *
              0.1f; // keep height of field smaller compared to width of x and z

    displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
    float z = cos(angle) * radius + displacement;
    model = glm::translate(model, glm::vec3(x, y, z));

    float rotAngle = (rand() % 360);
    model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

    // scale
    float scale = (rand() % 20) / 100.0f + 0.05;
    model = glm::scale(model, glm::vec3(scale));
    // 4. now add to list of matrices

    modelMatrices[i] = model;
  }
  auto cube = createRef<EMesh>(createRef<EBoxGeometry>(), mat);

  auto astroid = loader.loadModel("../models/rock/rock.obj", mat);
  astroid->enableInstanced(modelMatrices);
  /* astroid->scale(glm::vec3(2.0f, 1.0f, 3.0f)); */
  /* astroid->setRotation(45, 10, 0); */
  auto mars = loader.loadModel("../models/planet/planet.obj", mat);
  mat->uniforms["radius"] = radius;
  mars->setPosition(0, 0, 0);
  mars->setScale(4, 4, 4);
  astroid->setPosition(0, 0, 0);
  auto pnt = std::make_shared<PointLight>(
      glm::vec3(1.0f, 0.3f, 0.1f), glm::vec3(1.0f, 1.0f, 1.0f),
      glm::vec3(5, 15, 7), 1.20f, 0.045f, 0.0075f);
  auto pnt2 = std::make_shared<PointLight>(glm::vec3(0.0f, 1.0f, 0.1f),
                                           glm::vec3(1.0f, 0.1f, 1.0f),
                                           glm::vec3(-7, 3, 7), 10.0f);
  Ref<PointLight> pnt3 = createRef<PointLight>(glm::vec3(0.2f, 0.1f, 1.0f),
                                               glm::vec3(1.0f, 1.0f, 1.0f),
                                               glm::vec3(7, 5, 12), 3.0f);
  AmbientLight amb(glm::vec3(1.0f, 1.0f, 1.0f), 0.6);
  SpotLight spt({1.0f, 1.0f, 1.0f}, {1.0f, 1.0, 1.0}, {0.0f, 20.0f, 3.0f},
                {0.0f, -1.0f, 0.f}, 1.0f, glm::cos(glm::radians(30.0f)),
                glm::cos(glm::radians(50.0f)));

  zaWardu->add(astroid);
  zaWardu->add(mars);

  zaWardu->AddLight(std::make_shared<SpotLight>(spt));
  zaWardu->AddLight(pnt);
  /* zaWardu->AddLight(pnt2); */
  /* zaWardu->AddLight(pnt3); */
  /* zaWardu->loadCubeMaps({ */
  /*     "../assets/water_scene_cubeMap/right.jpg", */
  /*     "../assets/water_scene_cubeMap/left.jpg", */
  /*     "../assets/water_scene_cubeMap/top.jpg", */
  /*     "../assets/water_scene_cubeMap/bottom.jpg", */
  /*     "../assets/water_scene_cubeMap/front.jpg", */
  /*     "../assets/water_scene_cubeMap/back.jpg", */
  /* }); */
  /**/

  zaWardu->loadCubeMaps({

      "../assets/space_2/skybox_right1.png",
      "../assets/space_2/skybox_left2.png",
      "../assets/space_2/skybox_top3.png",
      "../assets/space_2/skybox_bottom4.png",
      "../assets/space_2/skybox_front5.png",
      "../assets/space_2/skybox_back6.png",

  });
  zaWardu->AddLight(std::make_shared<AmbientLight>(amb));
  float spiralRadius = 5.0f;
  float spiralHeight = 10.0f;
  float spiralSpeed = 1.0f;

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  while (nWindow.isOpen()) {
    float angle = glfwGetTime() * spiralSpeed; // Angle based on time and speed

    // Calculate x and z positions using sine and cosine
    float x = spiralRadius * cos(angle);
    float z = spiralRadius * sin(angle);
    float y = spiralHeight * (sin(angle / 2.0f) + 1.0f) /
              2.0f; // Adjust height to create the spiral effect
    /* pnt->setPosition(x, y, z); */
    /* pnt2->setPosition(x + 2.0f, y, z + 2.0f); // Offset for different lights
     */
    /* pnt3->setPosition(x + 4.0f, y, z + 4.0f); */
    mat->uniforms["time"] = glfwGetTime();
    rend.Render(zaWardu);
    nWindow.Update();
  }
  printf("Good Bye\n");

  return 0;
}
