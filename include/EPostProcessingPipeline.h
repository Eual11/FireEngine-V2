#ifndef _EPOSTPROCESSINGPIPELINE_
#define _EPOSTPROCESSINGPIPELINE_
#include "EFrameBuffer.h"
#include "EMesh.h"
#include "EQuadGeometry.h"
#include "Window.h"
#include <memory>

enum class PostProcessingEffect {
  Greyscale,
  Invert,
  GaussianBlurFull,
  GaussianBlurHorizontal,
  GaussianBlurVertical,
  Quantize,
  Downsample,
  Threshold,
};
struct EPostProcessingEffect {
  virtual void Apply(Window &, std::shared_ptr<EMesh> &, EFrameBuffer &,
                     EFrameBuffer &) = 0;
  virtual ~EPostProcessingEffect() = default;
  std::unique_ptr<Shader> effect;

  // protection to avoid writing and modifying stencil and depth buffers
  // when post processing effects are applied
  void LockDepthAndStencil();
  void UnlockDepthAndStencil();
  int internalEffectsCount = 1;
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

struct EGaussianBlurH : public EPostProcessingEffect {
  unsigned int radius = 1;
  void Apply(Window &, std::shared_ptr<EMesh> &, EFrameBuffer &,
             EFrameBuffer &) override;
  EGaussianBlurH(unsigned int radius = 1);
  ~EGaussianBlurH() override = default;
};
struct EGaussianBlurV : public EPostProcessingEffect {
  unsigned int radius = 1;
  void Apply(Window &, std::shared_ptr<EMesh> &, EFrameBuffer &,
             EFrameBuffer &) override;
  EGaussianBlurV(unsigned int radius = 1);
  ~EGaussianBlurV() override = default;
};
struct EQuantization : public EPostProcessingEffect {

  // number of quantization steps to map to
  unsigned int quantizationLevels = 64;
  void Apply(Window &, std::shared_ptr<EMesh> &, EFrameBuffer &,
             EFrameBuffer &) override;
  EQuantization(unsigned int steps = 64);
  ~EQuantization() override = default;
};

struct EDownsample : public EPostProcessingEffect {

  // downsampling factor, 2 means the resulting texture will be 50% smaller
  float factor = 2;
  EDownsample(float f = 2.0f);
  void Apply(Window &, std::shared_ptr<EMesh> &, EFrameBuffer &,
             EFrameBuffer &) override;
};

struct EThreshold : public EPostProcessingEffect {

  float threshold = 0.5;

  EThreshold(float threshold = 0.5);
  void Apply(Window &, std::shared_ptr<EMesh> &, EFrameBuffer &,
             EFrameBuffer &) override;
};
#endif
