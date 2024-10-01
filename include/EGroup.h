#ifndef __EGROUP__H
#define __EGROUP__H

#include "EObject3D.h"
#include <memory>
class EGroup : public EObject3D {

public:
  EGroup() : EObject3D() {}
  Type getType() override;
  void render(Shader &) override;
};
#endif
