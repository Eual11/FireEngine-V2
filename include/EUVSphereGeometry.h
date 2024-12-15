#ifndef __EUVSphereGeometry__
#define __EUVSphereGeometry__

#include "EBufferGeometry.h"
class EUVSphereGeometry : public EBufferGeometry {
public:
  EUVSphereGeometry(float radius = 1.0f, unsigned int stacks = 20,
                    unsigned int sectors = 20);

  unsigned int sectors; // vertical rings, thing of them as wedges
  unsigned int stacks;  // horizontal slices, think of them as slices of orange
  float radius;
};
#endif
