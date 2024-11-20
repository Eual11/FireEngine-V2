#ifndef _EPOSTPROCESSINGEFFECT_H
#define _EPOSTPROCESSINGEFFECT_H

#include "EFrameBuffer.h"
#include "EMesh.h"
#include "Window.h"
#include <memory>
struct EPostProcessingEffect {
  virtual void Apply(Window &, std::shared_ptr<EMesh> &, EFrameBuffer &,
                     EFrameBuffer &);
  std::unique_ptr<Shader> effect;

  virtual ~EPostProcessingEffect();
};

#endif
