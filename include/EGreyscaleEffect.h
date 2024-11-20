#ifndef __EGREYSCALEEFFECT__
#define __EGREYSCALEEFFECT__

#include "EPostProcessingPipeline.h"

struct EGreyscaleEffect : EPostProcessingEffect {

  void Apply(Window &, std::shared_ptr<EMesh> &, EFrameBuffer &,
             EFrameBuffer &) override;

  EGreyscaleEffect(float intensity = 1.0f);
};
#endif
