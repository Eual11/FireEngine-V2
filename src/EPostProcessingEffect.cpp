#include "../include/EPostProcessingPipeline.h"
#include <memory>

EPostProcessingPipeline::EPostProcessingPipeline(Window *window) {
  this->window = window;
  if (window) {
    framebuffers[0] = std::make_shared<EFrameBuffer>(window->getSize().w,
                                                     window->getSize().h);
    framebuffers[1] = std::make_shared<EFrameBuffer>(window->getSize().w,
                                                     window->getSize().h);

    window->addResizeCallback(
        [this](int w, int h) { framebuffers[0]->Resize(w, h); });

    window->addResizeCallback(
        [this](int w, int h) { framebuffers[1]->Resize(w, h); });
  }
}
void EPostProcessingPipeline::RenderToScreen() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
