#include "../include/EFrameBuffer.h"

EFrameBuffer::EFrameBuffer(int width, int height, bool onlyColor,
                           bool onlyDepth) {

  this->width = width;
  this->height = height;
  this->onlyColor = onlyColor;
  this->onlyDepth = onlyDepth;
  glGenFramebuffers(1, &fbo);

  glBindFramebuffer(GL_FRAMEBUFFER, fbo);

  // color attachment

  if (!onlyDepth) {

    glGenTextures(1, &color_attachment);
    glBindTexture(GL_TEXTURE_2D, color_attachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           color_attachment, 0);
    // depth_stencil renderbuffers

    if (!onlyColor) {
      glGenRenderbuffers(1, &rbo);
      glBindRenderbuffer(GL_RENDERBUFFER, rbo);

      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width,
                            height);

      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                                GL_RENDERBUFFER, rbo);
    }
  } else {
    glGenTextures(1, &depth_attachment);
    glBindTexture(GL_TEXTURE_2D,depth_attachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                           depth_attachment, 0);

    // disabling drawing and reading to save performance
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
  }
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {

    // dispose resources
    std::cerr << "Error Framebuffer not complete\n";
    glDeleteFramebuffers(1, &fbo);

    if (!onlyDepth) {

      glDeleteTextures(1, &color_attachment);

      if (!onlyColor) {
        glDeleteRenderbuffers(1, &rbo);
      }
    } else {
      glDeleteTextures(1, &depth_attachment);
    }
  } else {
    std::cout << "Farmebuffer Created\n";
  }
  GLenum err;

  while ((err = glGetError()) != GL_NO_ERROR) {
    std::cerr << "OpenGL Error: " << std::hex << err << std::endl;
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void EFrameBuffer::Resize(int new_w, int new_h) {
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  this->width = new_w;
  this->height = new_h;
  printf("(%d, %d)\n", new_w, new_h);

  // deleteing old color attachment and renderbuffers

  if (!onlyDepth) {

    glDeleteTextures(1, &color_attachment);
    color_attachment = 0;

    if (!onlyColor) {
      glDeleteRenderbuffers(1, &rbo);
      rbo = 0;
    }
    glGenTextures(1, &color_attachment);
    glBindTexture(GL_TEXTURE_2D, color_attachment);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, new_w, new_h, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           color_attachment, 0);

    if (!onlyColor) {

      glGenRenderbuffers(1, &rbo);

      glBindRenderbuffer(GL_RENDERBUFFER, rbo);

      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, new_w, new_h);

      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                                GL_RENDERBUFFER, rbo);
    }
  } else {

    glDeleteTextures(1, &depth_attachment);
    depth_attachment = 0;

    glGenTextures(1, &depth_attachment);
    glBindTexture(GL_TEXTURE_2D, depth_attachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                           depth_attachment, 0);

    // disabling drawing and reading to save performance
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
  }

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cerr << "Couldn't Resize Frame buffer\n";
  } else {
    std::cout << "Framebuffer Resized\n";
  }

  GLenum err;
  while ((err = glGetError()) != GL_NO_ERROR) {
    std::cerr << "OpenGL Error: " << std::hex << err << std::endl;
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
