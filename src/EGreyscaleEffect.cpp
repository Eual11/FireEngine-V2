#include "../include/EGreyscaleEffect.h"

EGreyscaleEffect::EGreyscaleEffect(float intensity) {
  effect = std::make_unique<Shader>("../shaders/vertex/quad_verts.glsl",
                                    "../shaders/fragment/fb.glsl");
  effect->setFloat("intensity", intensity);
}

void EGreyscaleEffect::Apply(Window &window, std::shared_ptr<EMesh> &quad,
                             EFrameBuffer &inBuffer, EFrameBuffer &outBuffer) {

  outBuffer.Bind();
  glDisable(GL_DEPTH_TEST);
  glViewport(0, 0, window.getSize().w, window.getSize().h);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glActiveTexture(GL_TEXTURE0);
  effect->setInt("screenTexture", 0);
  glBindTexture(GL_TEXTURE_2D, inBuffer.getTexture());
  window.UpdateUniforms(*(effect.get()));
  quad->render(*(effect.get()));
  glEnable(GL_DEPTH_TEST);
  outBuffer.Unbind();
}
