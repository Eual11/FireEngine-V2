#ifndef _EPOSTPROCESSINGPIPELINE_
#define _EPOSTPROCESSINGPIPELINE_
#include "EFrameBuffer.h"
#include "EPostProcessingEffect.h"
#include "Window.h"
#include <memory>
class EPostProcessingPipeline {

private:
  void RenderToScreen();
  std::vector<std::shared_ptr<EPostProcessingEffect>> effects;
  std::shared_ptr<EFrameBuffer> framebuffers[2];
  Window *window = nullptr;
  Shader screenShader = Shader("../shaders/vertex/quad_verts.glsl",
                               "../shaders/fragment/fb.glsl");

  int inputIndex = 0;

public:
  std::shared_ptr<EFrameBuffer> getInputFramebuffer() {
    return framebuffers[inputIndex];
  }
  EPostProcessingPipeline(Window *window);
  EPostProcessingPipeline() = default;
  void setWindow(Window *window) { this->window = window; }
  void addEffect(const std::shared_ptr<EPostProcessingEffect> &effect) {
    effects.push_back(effect);
  }
  void applyEffects();
};
#endif
