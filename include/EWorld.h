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
  // returns true if all materils
  bool shouldRecompileMaterials() const { return shaders_compiled; }
  // should be called after adding or removing from the scene
  //  after renderer compiles all shaders assosated with the scene it sets it to
  //  true
  void setRecompiled(bool val) { shaders_compiled = val; }
  std::map<std::string, std::vector<std::string>> skyboxes{
      {"Nebula",

       {
           "../assets/space_2/skybox_right1.png",
           "../assets/space_2/skybox_left2.png",
           "../assets/space_2/skybox_top3.png",
           "../assets/space_2/skybox_bottom4.png",
           "../assets/space_2/skybox_front5.png",
           "../assets/space_2/skybox_back6.png",
       }},
      {"Nebula 2",

       {
           "../assets/space_1/skybox_right1.png",
           "../assets/space_1/skybox_left2.png",
           "../assets/space_1/skybox_top3.png",
           "../assets/space_1/skybox_bottom4.png",
           "../assets/space_1/skybox_front5.png",
           "../assets/space_1/skybox_back6.png",
       }},
      {"Water Scene",
       {
           "../assets/water_scene_cubeMap/right.jpg",
           "../assets/water_scene_cubeMap/left.jpg",
           "../assets/water_scene_cubeMap/top.jpg",
           "../assets/water_scene_cubeMap/bottom.jpg",
           "../assets/water_scene_cubeMap/front.jpg",
           "../assets/water_scene_cubeMap/back.jpg",

       }

      },
      {"Blue Sky",
       {
           "../assets/blue_sky_cubeMaps/px.png",
           "../assets/blue_sky_cubeMaps/nx.png",
           "../assets/blue_sky_cubeMaps/py.png",
           "../assets/blue_sky_cubeMaps/ny.png",
           "../assets/blue_sky_cubeMaps/pz.png",
           "../assets/blue_sky_cubeMaps/nz.png",

       }

      },
      {"Interstellar",
       {
           "../assets/interstellar_skybox/xpos.png",
           "../assets/interstellar_skybox/xneg.png",
           "../assets/interstellar_skybox/ypos.png",
           "../assets/interstellar_skybox/yneg.png",
           "../assets/interstellar_skybox/zpos.png",
           "../assets/interstellar_skybox/zneg.png",

       }

      }

  };

private:
  std::vector<std::pair<std::shared_ptr<EModel>, Shader &>> Objects;
  unsigned int skyboxCubeMap;
  unsigned int skyboxVAO = 0;
  unsigned int skyboxVBO = 0;
  std::shared_ptr<Shader> skyboxShader = nullptr;
  // this tells the compiler if the World needs shader re-compilation
  // after modification
  bool shaders_compiled = false;
  std::string curCubeMapName = "";
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
  unsigned int loadCubeMaps(const std::string &name) {
    if (skyboxes.find(name) == skyboxes.end())
      return 0;
    curCubeMapName = name;
    return loadCubeMaps(skyboxes[name]);
  }
  std::string getCurCubeMap() const { return curCubeMapName; }
  std::vector<std::string> getCubemapNames() {
    std::vector<std::string> keys;
    std::transform(skyboxes.begin(), skyboxes.end(), std::back_inserter(keys),
                   [](const auto &pair) { return pair.first; });
    return keys;
  }
  void unloadCubeMaps();
  Window *getWindow() const { return attachedWindow; };
  void setWindow(Window *window) { attachedWindow = window; };
  void Render();
  void RenderSkybox();
  unsigned int getSkyboxVAO() const { return skyboxVAO; }
  unsigned int getSkyboxCubeMap() const { return skyboxCubeMap; }
  std::vector<std::shared_ptr<ELight>> Lights;
  void add(const std::shared_ptr<EObject3D> &obj) {
    EObject3D::add(obj);
    shaders_compiled = false;
  }
  void remove(const std::shared_ptr<EObject3D> &obj) {
    EObject3D::remove(obj);
    shaders_compiled = false;
  }
  void setInstanceUBO(unsigned int ubo, unsigned int count) override {};
};

#endif
