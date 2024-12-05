#include "../include/EPostProcessingPipeline.h"
#include <algorithm>
#include <memory>

void EPostProcessingEffect::LockDepthAndStencil() {
  glDisable(GL_STENCIL_TEST);
  glDisable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);
  glStencilMask(0x00);
}

void EPostProcessingEffect::UnlockDepthAndStencil() {

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_STENCIL_TEST);
  glStencilMask(0xFF);
  glDepthMask(GL_TRUE);
}

EPostProcessingPipeline::EPostProcessingPipeline(Window *window) {
  this->window = window;
  if (window) {
    framebuffers[0] = std::make_shared<EFrameBuffer>(
        window->getSize().w, window->getSize().h, false);
    framebuffers[1] = std::make_shared<EFrameBuffer>(
        window->getSize().w, window->getSize().h, false);
    auto geo = std::make_shared<EQuadGeometry>(1.0f, 1.0f);

    auto mat = std::make_shared<ShaderMaterial>(
        "../shaders/vertex/quad_verts.glsl", "../shaders/fragment/fb.glsl");
    quad = std::make_shared<EMesh>(geo, mat);
  }
}

void EPostProcessingPipeline::Init() {

  window->addResizeCallback(
      [this](int w, int h) { framebuffers[0]->Resize(w, h); });

  window->addResizeCallback(
      [this](int w, int h) { framebuffers[1]->Resize(w, h); });
}

void EPostProcessingPipeline::applyEffects() {

  // loop through the effects and apply them

  if (!window)
    return;

  for (const auto &effect : effects) {

    effect->Apply(*window, quad, *framebuffers[inputIndex].get(),
                  *framebuffers[outputIndex].get());
    std::swap(inputIndex, outputIndex);
  }

  // clearing both buffers
  //  render the finan output to the screen
  RenderToScreen();
}
void EPostProcessingPipeline::RenderToScreen() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDisable(GL_DEPTH_TEST);
  glViewport(0, 0, window->getSize().w, window->getSize().h);
  glClear(GL_COLOR_BUFFER_BIT);
  glActiveTexture(GL_TEXTURE0);
  screenShader.setInt("screenTexture", 0);
  glBindTexture(GL_TEXTURE_2D, framebuffers[inputIndex]->getTexture());
  window->UpdateUniforms(screenShader);
  quad->render(screenShader);
  glBindTexture(GL_TEXTURE_2D, 0);
  glEnable(GL_DEPTH_TEST);
}
EGreyscaleEffect::EGreyscaleEffect(float intensity) {
  effect = std::make_unique<Shader>("../shaders/vertex/quad_verts.glsl",
                                    "../shaders/fragment/Greyscale.glsl");
  effect->setFloat("intensity", intensity);
}

void EGreyscaleEffect::Apply(Window &window, std::shared_ptr<EMesh> &quad,
                             EFrameBuffer &inBuffer, EFrameBuffer &outBuffer) {
  // Disable writing to the stencil buffer

  LockDepthAndStencil();
  outBuffer.Bind();
  glViewport(0, 0, window.getSize().w, window.getSize().h);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glActiveTexture(GL_TEXTURE0);
  effect->setInt("screenTexture", 0);
  glBindTexture(GL_TEXTURE_2D, inBuffer.getTexture());
  window.UpdateUniforms(*(effect.get()));
  quad->render(*(effect.get()));
  outBuffer.Unbind();
  UnlockDepthAndStencil();
}
EInvertEffect::EInvertEffect() {
  effect = std::make_unique<Shader>("../shaders/vertex/quad_verts.glsl",
                                    "../shaders/fragment/Invert.glsl");
}

void EInvertEffect::Apply(Window &window, std::shared_ptr<EMesh> &quad,
                          EFrameBuffer &inBuffer, EFrameBuffer &outBuffer) {

  LockDepthAndStencil();

  outBuffer.Bind();
  glViewport(0, 0, window.getSize().w, window.getSize().h);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glActiveTexture(GL_TEXTURE0);
  effect->setInt("screenTexture", 0);
  glBindTexture(GL_TEXTURE_2D, inBuffer.getTexture());
  window.UpdateUniforms(*(effect.get()));
  quad->render(*(effect.get()));
  outBuffer.Unbind();

  UnlockDepthAndStencil();
}

EGaussianBlurH::EGaussianBlurH(unsigned int radius) {

  effect = std::make_unique<Shader>(
      "../shaders/vertex/quad_verts.glsl",
      "../shaders/fragment/GaussianBlurHorizontal.glsl");
  this->radius = radius;
}
EGaussianBlurV::EGaussianBlurV(unsigned int radius) {

  effect =
      std::make_unique<Shader>("../shaders/vertex/quad_verts.glsl",
                               "../shaders/fragment/GaussianBulrVertical.glsl");
  this->radius = radius;
}
void EGaussianBlurH::Apply(Window &window, std::shared_ptr<EMesh> &quad,
                           EFrameBuffer &inBuffer, EFrameBuffer &outBuffer) {

  LockDepthAndStencil();

  outBuffer.Bind();

  glViewport(0, 0, window.getSize().w, window.getSize().h);
  glClearColor(0.0f, 0.0f, 0.0f, 0.01);
  glClear(GL_COLOR_BUFFER_BIT);
  glActiveTexture(GL_TEXTURE0);
  effect->setInt("screenTexture", 0);
  effect->setInt("radius", radius);
  glBindTexture(GL_TEXTURE_2D, inBuffer.getTexture());
  window.UpdateUniforms(*(effect.get()));
  quad->render(*(effect.get()));
  outBuffer.Unbind();
  UnlockDepthAndStencil();
}
void EGaussianBlurV::Apply(Window &window, std::shared_ptr<EMesh> &quad,
                           EFrameBuffer &inBuffer, EFrameBuffer &outBuffer) {

  LockDepthAndStencil();

  outBuffer.Bind();

  glViewport(0, 0, window.getSize().w, window.getSize().h);
  glClearColor(0.0f, 0.0f, 0.0f, 0.01);
  glClear(GL_COLOR_BUFFER_BIT);
  glActiveTexture(GL_TEXTURE0);
  effect->setInt("screenTexture", 0);
  effect->setInt("radius", radius);
  glBindTexture(GL_TEXTURE_2D, inBuffer.getTexture());
  window.UpdateUniforms(*(effect.get()));
  quad->render(*(effect.get()));
  outBuffer.Unbind();
  UnlockDepthAndStencil();
}
EQuantization::EQuantization(unsigned int steps_) {
  this->quantizationLevels = steps_;

  effect = std::make_unique<Shader>("../shaders/vertex/quad_verts.glsl",
                                    "../shaders/fragment/Quantize.glsl");
}
void EQuantization::Apply(Window &window, std::shared_ptr<EMesh> &quad,
                          EFrameBuffer &inBuffer, EFrameBuffer &outBuffer) {

  LockDepthAndStencil();

  outBuffer.Bind();

  glViewport(0, 0, window.getSize().w, window.getSize().h);
  glClearColor(0.0f, 0.0f, 0.0f, 0.01);
  glClear(GL_COLOR_BUFFER_BIT);
  glActiveTexture(GL_TEXTURE0);
  effect->setInt("screenTexture", 0);
  effect->setInt("levels", quantizationLevels);
  glBindTexture(GL_TEXTURE_2D, inBuffer.getTexture());
  window.UpdateUniforms(*(effect.get()));
  quad->render(*(effect.get()));
  outBuffer.Unbind();

  UnlockDepthAndStencil();
}

EDownsample::EDownsample(float f) {
  this->factor = f;
  effect = std::make_unique<Shader>("../shaders/vertex/quad_verts.glsl",
                                    "../shaders/fragment/Downsample.glsl");
}
void EDownsample::Apply(Window &window, std::shared_ptr<EMesh> &quad,
                        EFrameBuffer &inBuffer, EFrameBuffer &outBuffer) {
  LockDepthAndStencil();
  outBuffer.Bind();
  glViewport(0.0, 0.0, window.getSize().w, window.getSize().h);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glActiveTexture(GL_TEXTURE0);
  effect->setInt("screenTexture", 0);
  effect->setFloat("factor", factor);
  glBindTexture(GL_TEXTURE_2D, inBuffer.getTexture());
  window.UpdateUniforms(*(effect.get()));
  quad->render(*(effect.get()));

  outBuffer.Unbind();
  UnlockDepthAndStencil();
}
EThreshold::EThreshold(float threshold) {
  this->threshold = threshold;

  effect = std::make_unique<Shader>("../shaders/vertex/quad_verts.glsl",
                                    "../shaders/fragment/Threshold.glsl");
}
void EThreshold::Apply(Window &window, std::shared_ptr<EMesh> &quad,
                       EFrameBuffer &inBuffer, EFrameBuffer &outBuffer) {
  LockDepthAndStencil();
  outBuffer.Bind();
  glViewport(0.0, 0.0, window.getSize().w, window.getSize().h);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glActiveTexture(GL_TEXTURE0);
  effect->setInt("screenTexture", 0);
  effect->setFloat("threshold", threshold);
  glBindTexture(GL_TEXTURE_2D, inBuffer.getTexture());
  window.UpdateUniforms(*(effect.get()));
  quad->render(*(effect.get()));

  outBuffer.Unbind();
  UnlockDepthAndStencil();
}
EBloom::EBloom(int w, int h, float threshold) {

  this->threshold = threshold;
  this->width = w;
  this->height = h;
  framebuffers[0] = std::make_shared<EFrameBuffer>(w, h, true);
  framebuffers[1] = std::make_shared<EFrameBuffer>(w, h, true);
  effects[0] = std::make_unique<Shader>("../shaders/vertex/quad_verts.glsl",
                                        "../shaders/fragment/Threshold.glsl");

  effects[1] = std::make_unique<Shader>("../shaders/vertex/quad_verts.glsl",
                                        "../shaders/fragment/Downsample.glsl");
  effects[2] = std::make_unique<Shader>(
      "../shaders/vertex/quad_verts.glsl",
      "../shaders/fragment/GaussianBlurHorizontal.glsl");
  effects[3] =
      std::make_unique<Shader>("../shaders/vertex/quad_verts.glsl",
                               "../shaders/fragment/GaussianBulrVertical.glsl");

  copyScene =
      std::make_unique<Shader>("../shaders/vertex/quad_verts.glsl",
                               "../shaders/fragment/displayScreen.glsl");

  mix = std::make_unique<Shader>("../shaders/vertex/quad_verts.glsl",
                                 "../shaders/fragment/MixBloom.glsl");
}
void EBloom::Apply(Window &window, std::shared_ptr<EMesh> &quad,
                   EFrameBuffer &inBuffer, EFrameBuffer &outBuffer) {

  LockDepthAndStencil();

  if (window.getSize().w != width || window.getSize().h != height) {
    // change in width and height
    framebuffers[0]->Resize(window.getSize().w, window.getSize().h);
    framebuffers[1]->Resize(window.getSize().w, window.getSize().h);
    width = window.getSize().w;
    height = window.getSize().h;
  }

  // copy scene to input framebuffer;
  framebuffers[inputIndex]->Bind();

  glViewport(0, 0, window.getSize().w, window.getSize().h);
  glClear(GL_COLOR_BUFFER_BIT);
  glActiveTexture(GL_TEXTURE0);
  copyScene->setInt("screenTexture", 0);
  glBindTexture(GL_TEXTURE_2D, inBuffer.getTexture());
  window.UpdateUniforms(*(copyScene.get()));
  quad->render(*(copyScene.get()));
  glBindTexture(GL_TEXTURE_2D, 0);

  int idx = 0;
  framebuffers[inputIndex]->Unbind();

  for (const auto &effect : effects) {
    if (idx == 0) {
      effect->setFloat("threshold", 0.4);
    }
    if (idx == 1) {
      effect->setFloat("factor", 2);
    }
    if (idx == 2) {
      effect->setInt("radius", 16);
    }
    if (idx == 3) {
      effect->setInt("radius", 16);
    }
    ApplyLocal(window, quad, *framebuffers[inputIndex].get(),
               *framebuffers[outputIndex].get(), effect.get());
    std::swap(inputIndex, outputIndex);
    idx++;
  }

  // mxing the result with the scene
  outBuffer.Bind();

  glViewport(0, 0, window.getSize().w, window.getSize().h);
  glClear(GL_COLOR_BUFFER_BIT);
  glActiveTexture(GL_TEXTURE0);
  mix->setInt("screenTexture", 0);
  glBindTexture(GL_TEXTURE_2D, inBuffer.getTexture());

  glActiveTexture(GL_TEXTURE1);
  mix->setInt("bloomTexture", 1);
  glBindTexture(GL_TEXTURE_2D, framebuffers[inputIndex]->getTexture());

  window.UpdateUniforms(*(mix.get()));
  quad->render(*(mix.get()));
  glBindTexture(GL_TEXTURE_2D, 0);

  outBuffer.Unbind();
  UnlockDepthAndStencil();
}

void EBloom::ApplyLocal(Window &window, std::shared_ptr<EMesh> &quad,
                        EFrameBuffer &inBuffer, EFrameBuffer &outBuffer,
                        Shader *effect) {

  outBuffer.Bind();
  glViewport(0, 0, window.getSize().w, window.getSize().h);
  glClear(GL_COLOR_BUFFER_BIT);
  glActiveTexture(GL_TEXTURE0);
  effect->setInt("screenTexture", 0);
  glBindTexture(GL_TEXTURE_2D, inBuffer.getTexture());
  window.UpdateUniforms(*(effect));
  quad->render(*(effect));
  glBindTexture(GL_TEXTURE_2D, 0);
  outBuffer.Unbind();
}
