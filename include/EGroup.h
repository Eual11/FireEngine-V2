#ifndef __EGROUP__H
#define __EGROUP__H

#include "EObject3D.h"
#include <memory>
class EGroup : public EObject3D {

public:
  EGroup() : EObject3D() {}
  Type getType() override;
  void render(Shader &) override;
  void setInstanceUBO(unsigned int ubo, unsigned int count) override {

    for (auto &child : children) {
      child->setInstanceUBO(ubo, count);
    }
  };
};
#endif
