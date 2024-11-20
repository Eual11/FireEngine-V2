#include "../include/EPostProcessingPipeline.h"
#include <memory>

EPostProcessingPipeline::EPostProcessingPipeline(Window *window) {
  this->window = window;
  if (window) {
    framebuffers[0] = std::make_shared<EFrameBuffer>(window->getSize().w,
                                                     window->getSize().h);
    framebuffers[1] = std::make_shared<EFrameBuffer>(window->getSize().w,
                                                     window->getSize().h);
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
                  *framebuffers[inputIndex].get());
    std::swap(inputIndex, outputIndex);
  }
  // render the finan output to the screen
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
  glEnable(GL_DEPTH_TEST);
  glBindTexture(GL_TEXTURE_2D, 0);
}
