
#ifndef __EBOXGEOMETRY__
#define __EBOXGEOMETRY__
#include "EBufferGeometry.h"

class EBoxGeometry : public EBufferGeometry {
public:
  float width;
  float height;

  EBoxGeometry(float width = 1.0f, float height = 1.0f, float depth = 1.0f);
};
class EPlaneGeometry : public EBufferGeometry {

public:
  float width;
  float height;
  EPlaneGeometry(float width = 1.0f, float height = 1.0f);
};
#endif
