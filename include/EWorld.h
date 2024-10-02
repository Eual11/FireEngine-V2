#ifndef _WORLD_H
#define _WORLD_H
#include "ELight.h"
#include "EModel.h"
#include "Shader.h"
#include "Window.h"
#include "stb_image.h"
#include <memory>
#include <vector>

#define MAX_LIGHT_COUNT 10
class EWorld {

private:
  std::vector<std::pair<std::shared_ptr<EModel>, Shader &>> Objects;
  std::vector<std::shared_ptr<ELight>> Lights;
  unsigned int skyboxCubeMap;
  unsigned int skyboxVAO = 0;
  bool hasSkyBox = false;
  std::shared_ptr<Shader> skyboxShader = nullptr;
  // TODO: find replacement for this raw ptr
  Window *attachedWindow = nullptr;

public:
  EWorld() = default;
  EWorld(Window *window) { attachedWindow = window; }
  void AddObject(std::shared_ptr<EModel>, Shader &);
  void AddLight(std::shared_ptr<ELight>);
  unsigned int loadCubeMaps(std::vector<std::string>);
  Window *getWindow() const { return attachedWindow; };
  void setWindow(Window *window) { attachedWindow = window; };
  void Render();
  void RenderSkybox();
};

#endif
