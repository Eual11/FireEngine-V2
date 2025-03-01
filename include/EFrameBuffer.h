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
  unsigned int depth_attachment =0;
  bool onlyColor = false;
  bool onlyDepth = false;

public:
  EFrameBuffer(int width, int height, bool onlyColor = false, bool onlyDepth=false);
  unsigned int getTexture() const { return color_attachment; }
  void Bind() const { glBindFramebuffer(GL_FRAMEBUFFER, fbo); }
  void Unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
  void Resize(int new_w, int new_h);
  int width;
  int height;
  // tells if the framebuffer has only a color color_attachment
  bool isColorOnly() const { return onlyColor; }
};
#endif
