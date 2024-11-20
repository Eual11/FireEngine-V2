#ifndef _EPOSTPROCESSINGPIPELINE_
#define _EPOSTPROCESSINGPIPELINE_
#include "EFrameBuffer.h"
#include "EMesh.h"
#include "EQuadGeometry.h"
#include "Window.h"
#include <memory>

enum class PostProcessingEffect { Greyscale, Invert, GaussianBlur3x3 };
struct EPostProcessingEffect {
  virtual void Apply(Window &, std::shared_ptr<EMesh> &, EFrameBuffer &,
                     EFrameBuffer &) = 0;
  virtual ~EPostProcessingEffect() = default;
  std::unique_ptr<Shader> effect;
};

class EPostProcessingPipeline {

private:
  void RenderToScreen();
  std::vector<std::shared_ptr<EPostProcessingEffect>> effects;
  std::shared_ptr<EFrameBuffer> framebuffers[2];
  Window *window = nullptr;
  Shader screenShader = Shader("../shaders/vertex/quad_verts.glsl",
                               "../shaders/fragment/displayScreen.glsl");

  std::shared_ptr<EMesh> quad;
  int inputIndex = 0;
  int outputIndex = 1;

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
  void Init();
};

struct EGreyscaleEffect : public EPostProcessingEffect {

  void Apply(Window &, std::shared_ptr<EMesh> &, EFrameBuffer &,
             EFrameBuffer &) override;
  ~EGreyscaleEffect() override = default;

  EGreyscaleEffect(float intensity = 1.0f);
};
struct EInvertEffect : public EPostProcessingEffect {

  void Apply(Window &, std::shared_ptr<EMesh> &, EFrameBuffer &,
             EFrameBuffer &) override;

  EInvertEffect();
  ~EInvertEffect() override = default;
};
#endif
