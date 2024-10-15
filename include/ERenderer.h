#ifndef __ERENDERER_H__
#define __ERENDERER_H__

#include "EMaterial.h"
#include "EObject3D.h"
#include "EWorld.h"
#include "Window.h"
class ERenderer {
public:
  ERenderer();
  ERenderer(Window *window);
  void setWindow(Window *_win) { window = _win; }
  void EnableDepthTesting();
  void DisableDepthTesting();
  void setDepthTestFunc(unsigned int);
  void Render(std::shared_ptr<EWorld> &);

private:
  Window *window = nullptr;
  bool depthTestingEnabled_;

  //
  // this will stored every shadder mapped to the objects
  std::map<std::shared_ptr<EObject3D>, std::shared_ptr<Shader>> shader_map;
  // shaders compiled so far
  std::map<std::string, std::shared_ptr<Shader>> compiled_shaders;

  bool shaders_compiled = false;

  void CompileShaders(std::shared_ptr<EObject3D>);
  void CalculateLighting(const std::shared_ptr<EWorld> &, Shader &);
  void setupLight(std::shared_ptr<ELight> light, Shader &);
  void FetchShaderAndRender(const std::shared_ptr<EWorld> &world,
                            std::shared_ptr<EObject3D> &obj);
  // default material sources
  //
  std::map<MaterialType, std::string> default_fragment_shader_sources{
      {MaterialType::MaterialType_Basic, "../shaders/fragment/basic2.glsl"},
      {MaterialType::MaterialType_Phong, "../shaders/fragment/basic.glsl"},
      {MaterialType::MaterialType_Normal,
       "../shaders/fragment/normal_material.glsl"}};
  std::string default_vertex_shader_source = "../shaders/vertex/basic.glsl";

  std::string default_skybox_shader_source = "../shaders/vertex/sykbox.glsl";
  void CompileMeshShader(std::shared_ptr<EObject3D> &);
  void RenderSkybox(std::shared_ptr<EWorld> &world);
  void CompileWorldShader(std::shared_ptr<EWorld> &object);
  void CompileModelShader(std::shared_ptr<EObject3D> &object);
};
#endif
