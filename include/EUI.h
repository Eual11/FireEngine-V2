#include "../_dep/imgui-src/imgui.h"
#include "Camera.h"
#include "EBoxGeometry.h"
#include "ELight.h"
#include "EModelLoader.h"
#include "ERenderer.h"
#include "EUVSphereGeometry.h"
#include "EWorld.h"
#include "Window.h"

struct CameraState {

  glm::vec3 Poition;
  glm::vec3 Orientation;
  glm::vec2 clipPlanes;
  float max_speed;
  float acceleration;
  float Fov;
  bool updateByUI = false;
};
struct RendererState {

  bool enableDepthTesting;
  bool enableStencilTesting;
  bool enableGrid;
  bool enableNormalMapping;
  bool update;
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
  EModelLoader loader;

  // a weak pointer to the world
  std::shared_ptr<EWorld> World;
  std::shared_ptr<EObject3D> curSelectedObject;
  std::shared_ptr<ELight> curSelectedLight;

  bool vsyncEnabled;
};

struct ObjectLoaderEntry {
  std::string entry_name;
  std::function<void(EngineState &state)> action;
};

void UpdateEngine(Window &window, EngineState &state);
void UpdateCamera(Camera &camera, EngineState &state);
void UpdateRenderer(ERenderer &renderer, EngineState &state);
void UpdateWorld(EWorld &world, EngineState &state);
void RenderUI(EngineState &state);
void StartUIFrame();
void TraverseObjects(EngineState &state, std::shared_ptr<EObject3D> obj);
void TraverseLights(EngineState &state);
void EndUIFrame();

void AddCube(EngineState &state);
void AddPlane(EngineState &state);
void AddSphere(EngineState &state);
void AddTorus(EngineState &state);
