#ifndef _WORLD_H
#define _WORLD_H
#include "ELight.h"
#include "EMaterial.h"
#include "EModel.h"
#include "EObject3D.h"
#include "Shader.h"
#include "Window.h"
#include "stb_image.h"
#include <map>
#include <memory>
#include <vector>

#define MAX_LIGHT_COUNT 10
class EWorld : public EObject3D {
public:
  bool hasSkyBox = false;

private:
  std::vector<std::pair<std::shared_ptr<EModel>, Shader &>> Objects;
  unsigned int skyboxCubeMap;
  unsigned int skyboxVAO = 0;
  std::shared_ptr<Shader> skyboxShader = nullptr;
  // TODO: find replacement for this raw ptr
  Window *attachedWindow = nullptr;

  //
  // this will stored every shadder mapped to the objects

public:
  EWorld() = default;
  ~EWorld() override = default;

  Type getType() override { return Type::World; }
  EWorld(Window *window) { attachedWindow = window; }
  void AddObject(std::shared_ptr<EModel>, Shader &);
  void AddLight(std::shared_ptr<ELight>);
  void render(Shader &) override;
  unsigned int loadCubeMaps(std::vector<std::string>);
  Window *getWindow() const { return attachedWindow; };
  void setWindow(Window *window) { attachedWindow = window; };
  void Render();
  void RenderSkybox();
  unsigned int getSkyboxVAO() const { return skyboxVAO; }
  unsigned int getSkyboxCubeMap() const { return skyboxCubeMap; }
  std::vector<std::shared_ptr<ELight>> Lights;
};

#endif
