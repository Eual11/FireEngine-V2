#ifndef __ERENDERER_H__
#define __ERENDERER_H__

#include "EBufferGeometry.h"
#include "EFrameBuffer.h"
#include "ELight.h"
#include "EMaterial.h"
#include "EModel.h"
#include "EObject3D.h"
#include "EPostProcessingPipeline.h"
#include "EWorld.h"
#include "Window.h"

enum class PolyMode { FILL, LINE, POINT };
class ERenderer {
public:
  ERenderer();
  ERenderer(Window *window);
  void setWindow(Window *_win) { window = _win; }
  void EnableDepthTesting();
  void EnableStencilTesting();
  bool getDepthTesting() const { return depthTestingEnabled; }
  bool getStencilTesting() const { return stencilTestingEnabled; }
  bool girdEnabled() const { return grid; }
  bool getGammaCorrection() const { return gammaCorrection; }
  bool getNormalMapping() const { return normalMapping; }
  void setNormalMapping(bool val) { normalMapping = val; }
  void setGammaCorrection(bool val) { gammaCorrection = val; }
  inline void EnableGrid() {
    if (grid)
      return;
    grid = true;
  }
  inline void DisableGrid() {
    if (!grid)
      return;
    grid = false;
  }
  // HACK: outlines should be a property of mesh/model not the renderer itself
  // but we will leave this for now
  void EnableOutlines() {
    if (!outlinesEnabled)
      outlinesEnabled = true;
  }
  void DisableOutlines() {
    if (outlinesEnabled)
      outlinesEnabled = false;
  }
  void DisableDepthTesting();
  void DisableStencilTesting();
  void inline setClearColor(float r, float g, float b, float a = 1.0f);
  void setDepthTestFunc(unsigned int);
  void addEffect(const std::shared_ptr<EPostProcessingEffect> &);
  void addEfect(PostProcessingEffect);
  PolyMode getPolyMode() const { return polymode; }
  void setPolyMode(PolyMode mode) {

    polymode = mode;

    switch (polymode) {
    case PolyMode::FILL: {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      break;
    }
    case PolyMode::LINE: {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      break;
    }
    case PolyMode::POINT: {
      glPolygonMode(GL_FRONT_AND_BACK, POINT);
      break;
    }
    }
  }

  void Render(std::shared_ptr<EWorld> &);

private:
  Window *window = nullptr;
  bool depthTestingEnabled = false;
  bool stencilTestingEnabled = false;
  bool outlinesEnabled = false;
  bool grid = true;
  bool normalMapping = true;
  bool gammaCorrection = true;
  glm::vec4 clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
  unsigned int clearBit = GL_COLOR_BUFFER_BIT;
  PolyMode polymode = PolyMode::FILL;

  std::shared_ptr<Shader> outlineShader = nullptr;
  float outlineSize = 1.2;
  /* std::unique_ptr<EFrameBuffer> framebuffer; */

  std::shared_ptr<EMesh> fGrid;
  std::shared_ptr<EModel> directional_helper;
  std::shared_ptr<EObject3D> pointlight_helper;
  std::shared_ptr<EModel> spotlight_helper;

  EPostProcessingPipeline effectPipeline;

  //
  // this will stored every shadder mapped to the objects
  std::map<std::shared_ptr<EObject3D>, std::shared_ptr<Shader>> shader_map;
  // shaders compiled so far
  std::map<std::string, std::shared_ptr<Shader>> compiled_shaders;

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
       "../shaders/fragment/normal_material.glsl"},
      {MaterialType_PBR, "../shaders/fragment/PBR.glsl"}

  };
  std::string default_vertex_shader_source = "../shaders/vertex/basic.glsl";

  std::string default_skybox_shader_source = "../shaders/vertex/sykbox.glsl";
  void CompileMeshShader(std::shared_ptr<EObject3D> &);
  void RenderSkybox(std::shared_ptr<EWorld> &world);
  void CompileWorldShader(std::shared_ptr<EWorld> &object);
  void CompileModelShader(std::shared_ptr<EObject3D> &object);
};
#endif
