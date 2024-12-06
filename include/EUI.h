#include "../_dep/imgui-src/imgui.h"
#include "Camera.h"
#include "EWorld.h"
#include "Window.h"

struct CameraState {

  glm::vec3 Poition;
  glm::vec3 Orientation;
  glm::vec2 clipPlanes;
  float Fov;
  bool updateByUI = false;
};
struct RendererState {};
struct SceneState {};
struct EngineState {
  CameraState cameraState;
  RendererState rendererState;
  bool vsyncEnabled;
};

void UpdateEngine(Window &window, EngineState &state);
void UpdateCamera(Camera &camera, EngineState &state);
void UpdateRenderer(EWorld &world, EngineState &state);
void RenderUI(EngineState &state);
void StartUIFrame();
void EndUIFrame();
