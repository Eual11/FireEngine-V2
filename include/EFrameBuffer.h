#ifndef _EFRAMEBUFFER_H
#define _EFRAMEBUFFER_H
#include "EBufferGeometry.h"
#include "Error.h"
#include <iostream>
class EFrameBuffer {
private:
  unsigned int fbo = 0;
  unsigned int rbo = 0;
  unsigned int color_attachment = 0;

public:
  EFrameBuffer(int width, int height);
  unsigned int getTexture() const { return color_attachment; }
  void Bind() const { glBindFramebuffer(GL_FRAMEBUFFER, fbo); }
  void Unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
  void Resize(int new_w, int new_h);
  int width;
  int height;
};
#endif
