#include "../include/EBoxGeometry.h"
#include "../include/EModelLoader.h"
#include "../include/ERenderer.h"
#include "../include/EUI.h"
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
glm::vec3 camPos(0.0f, 0.0f, 10.0f);
glm::vec3 camFront(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);

glm::vec3 lightPos(0.0f, 0.0f, 8.0f);

// TODO: somehow the child mesh's frag shaders is being executed
int main() {
  Camera camera(camPos, cameraUp, fYaw, fPitch);
  auto cameraPtr = std::make_shared<Camera>(camera);
  Window nWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Firev2", false);
  nWindow.BindCamera(cameraPtr);
  nWindow.EnableCursor();

  auto zaWardu = std::make_shared<EWorld>(&nWindow);
  ERenderer rend(&nWindow);

  UniformMap uniforms = {{"time", 0.0f}, {"uAmp", 2.0f}};
  auto mat = std::make_shared<ShaderMaterial>("../shaders/vertex/basic.glsl",
                                              "../shaders/fragment/basic.glsl",
                                              uniforms);

  EModelLoader loader;
  auto astroid = loader.loadModel("../models/rock/rock.obj", mat);
  auto sun = loader.loadModel("../models/planet/sun/scene.gltf", mat);
  auto helm =
      loader.loadModel("../models/survival_guitar_backpack/scene.gltf", mat);
  sun->setPosition(0, 0, 0);
  helm->setPosition(0, 0, 0);
  astroid->setPosition(0, 0, 0);
  auto pnt = createRef<PointLight>(
      glm::vec3(1.0f, 1.0f, 0.9f), glm::vec3(1.0f, 1.0f, 1.0f),
      glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, 0.01f, 0.01f);

  pnt->setPosition(0, 8, 0);
  AmbientLight amb(glm::vec3(1.0f, 1.0f, 1.0f), 0.7);
  zaWardu->AddLight(pnt);
  zaWardu->add(helm);

  zaWardu->loadCubeMaps("Nebula");
  zaWardu->AddLight(std::make_shared<AmbientLight>(amb));
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  EngineState state;

  // Renderer States

  state.rendererState.enableDepthTesting = rend.getDepthTesting();
  state.rendererState.enableStencilTesting = rend.getStencilTesting();
  state.rendererState.polymode = rend.getPolyMode();
  state.rendererState.enableGrid = rend.girdEnabled();
  state.rendererState.enableNormalMapping = rend.getNormalMapping();

  // Camera States
  state.cameraState.Poition = camera.getPosition();
  state.cameraState.Orientation = camera.getOrientation();
  state.cameraState.clipPlanes = camera.getNearFarPlanes();
  state.cameraState.Fov = camera.getFov();
  state.vsyncEnabled = nWindow.getVsyncState();

  // scene/world state
  state.worldstate.skyboxEnabled = zaWardu->hasSkyBox;
  state.worldstate.skyboxReloaded = false;
  state.worldstate.cubeMaps = zaWardu->getCubemapNames();

  state.worldstate.skyboxEnabled = zaWardu->hasSkyBox;
  state.worldstate.skyboxReloaded = false;
  state.worldstate.cubeMaps = zaWardu->getCubemapNames();

  // passing world ptr
  state.World = zaWardu;
  zaWardu->setName("zaWardu");
  auto it =
      std::find(state.worldstate.cubeMaps.begin(),
                state.worldstate.cubeMaps.end(), zaWardu->getCurCubeMap());
  if (it != state.worldstate.cubeMaps.end()) {
    state.worldstate.curSkyboxIdx =
        std::distance(state.worldstate.cubeMaps.begin(), it);
  } else {
    // Handle the case where the current cube map is not found
    state.worldstate.curSkyboxIdx = -1; // or some invalid index
  }
  while (nWindow.isOpen()) {

    StartUIFrame();

    mat->uniforms["time"] = glfwGetTime();
    rend.Render(state.World);

    RenderUI(state);
    EndUIFrame();

    UpdateEngine(nWindow, state);
    UpdateRenderer(rend, state);
    UpdateWorld(*(zaWardu.get()), state);
    UpdateCamera(*(cameraPtr.get()), state);
    nWindow.Update();
  }
  printf("Good Bye\n");

  return 0;
  ;
}
