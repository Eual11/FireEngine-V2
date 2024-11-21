#include "../include/EModelLoader.h"
#include "../include/ERenderer.h"
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
glm::vec3 camPos(0.0f, 0.0f, 17.0f);
glm::vec3 camFront(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);

glm::vec3 lightPos(0.0f, 0.0f, 8.0f);

// TODO: somehow the child mesh's frag shaders is being executed
int main() {

  Camera camera(camPos, cameraUp, fYaw, fPitch);
  auto cameraPtr = std::make_shared<Camera>(camera);
  /* auto windowPtr =  */
  Window nWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Firev2", false);
  nWindow.BindCamera(cameraPtr);
  // creating some shaders

  auto zaWardu = std::make_shared<EWorld>(&nWindow);
  ERenderer rend(&nWindow);

  /* rend.addEfect(PostProcessingEffect::GaussianBlur); */
  /* rend.addEfect(PostProcessingEffect::Invert); */
  rend.addEfect(PostProcessingEffect::Quantize);
  rend.addEfect(PostProcessingEffect::Invert);

  // TODO: tidy this up, it is disguting
  UniformMap uniforms = {{"time", 0.0f}, {"uAmp", 2.0f}};
  std::filesystem::path path("../models/scene.gltf");

  auto tex = ETexture::load(
      "../models/textures/3DLABbg_UV_Map_Checker_01_2048x2048.jpg");
  uniforms["tex"] = tex;
  EModelLoader loader;
  /* auto mat = std::make_shared<NormalMaterial>(NormalMaterial()); */
  auto mat = std::make_shared<ShaderMaterial>("../shaders/vertex/basic.glsl",
                                              "../shaders/fragment/basic2.glsl",
                                              uniforms);
  auto suzan = loader.loadModel(
      std::filesystem::absolute("../models/cube.obj").string(), mat);

  suzan->setPosition(0, 0, 4);
  /* suzan->setScale(0.02, 0.02, 0.02); */
  suzan->setRotation(0.0f, 45.0f, 0);
  auto helm = loader.loadModel(
      std::filesystem::absolute("../models/DamagedHelmet.gltf").string(), mat);

  helm->setPosition(10, 1, 0);
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

  // some funny demo for box geometry
  /**/
  /* float radius = 10.0f; */
  /* for (float theta = 0; theta < 2 * glm::pi<float>(); theta += 0.2) { */
  /**/
  /*   auto box = std::make_shared<EBoxGeometry>(); */
  /*   auto newMesh = std::make_shared<EMesh>(box, mat); */
  /*   newMesh->setPosition(radius * glm::cos(theta), radius * glm::sin(theta),
   * 5); */
  /*   zaWardu->add(newMesh); */
  /* } */
  /* for (float theta = 0; theta < 2 * glm::pi<float>(); theta += 0.2) { */
  /**/
  /*   auto box = std::make_shared<EBoxGeometry>(); */
  /*   auto newMesh = std::make_shared<EMesh>(box, mat); */
  /*   newMesh->setPosition(radius * glm::cos(theta), 0, radius *
   * glm::sin(theta)); */
  /*   zaWardu->add(newMesh); */
  /* } */
  zaWardu->add(helm);
  /* zaWardu->add(suzan); */

  zaWardu->AddLight(std::make_shared<SpotLight>(spt));
  zaWardu->AddLight(pnt);
  zaWardu->AddLight(pnt2);
  zaWardu->AddLight(pnt3);
  zaWardu->loadCubeMaps({
      "../assets/water_scene_cubeMap/right.jpg",
      "../assets/water_scene_cubeMap/left.jpg",
      "../assets/water_scene_cubeMap/top.jpg",
      "../assets/water_scene_cubeMap/bottom.jpg",
      "../assets/water_scene_cubeMap/front.jpg",
      "../assets/water_scene_cubeMap/back.jpg",
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
    pnt->setPosition(x, y, z);
    pnt2->setPosition(x + 2.0f, y, z + 2.0f); // Offset for different lights
    pnt3->setPosition(x + 4.0f, y, z + 4.0f);
    mat->uniforms["time"] = glfwGetTime();
    rend.Render(zaWardu);
    nWindow.Update();
  }
  printf("Good Bye\n");

  return 0;
}
