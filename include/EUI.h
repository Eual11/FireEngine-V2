#include "../_dep/imgui-src/imgui.h"
#include "Camera.h"
#include "ERenderer.h"
#include "EWorld.h"
#include "Window.h"

struct CameraState {

  glm::vec3 Poition;
  glm::vec3 Orientation;
  glm::vec2 clipPlanes;
  float Fov;
  bool updateByUI = false;
};
struct RendererState {

  bool enableDepthTesting;
  bool enableStencilTesting;
  PolyMode polymode;
};
struct WorldState {
  bool skyboxEnabled;
  int curSkyboxIdx;
  std::vector<std::string> cubeMaps;
  bool skyboxReloaded;
};
struct EngineState {
  CameraState cameraState;
  RendererState rendererState;
  WorldState worldstate;
  bool vsyncEnabled;
};

void UpdateEngine(Window &window, EngineState &state);
void UpdateCamera(Camera &camera, EngineState &state);
void UpdateRenderer(ERenderer &renderer, EngineState &state);
void UpdateWorld(EWorld &world, EngineState &state);
void RenderUI(EngineState &state);
void StartUIFrame();
void EndUIFrame();
