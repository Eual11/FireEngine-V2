#include "../include/EUVSphereGeometry.h"

EUVSphereGeometry::EUVSphereGeometry(float radius, unsigned int stacks,
                                     unsigned int sectors) {
  // generate vertices

  this->sectors = sectors;
  this->stacks = stacks;
  this->radius = radius;
  this->name = "UVSphere";
  float inver_rad = 1.0f / radius;
  for (unsigned int i = 0; i < stacks; i++) {

    float phi = (glm::pi<float>() * i) / (stacks - 1);
    for (unsigned int j = 0; j < sectors; j++) {

      float theta = (2.0f * glm::pi<float>() * j) / sectors;

      float x = radius * glm::sin(phi) * glm::cos(theta);
      float y = radius * glm::sin(phi) * glm::sin(theta);
      float z = radius * cos(phi);

      // tex coords
      float u = (float)(i) / (stacks - 1);
      float v = (float)(j) / (sectors);

      // normals
      float nx = x * inver_rad;
      float ny = y * inver_rad;
      float nz = z * inver_rad;

      vertices.push_back({{x, y, z}, {nx, ny, nz}, {u, v}});
    }
  }

  // generate indices

  // stacks-1, sectors-1 because we are exclusding the last stack and sectors
  // because they already overlap with the first stack and sector
  for (unsigned int i = 0; i < stacks - 1; i++) {

    for (unsigned int j = 0; j <= sectors; j++) {

      int first = i * (sectors) + j;
      int second = (i + 1) * (sectors) + j;

      int third = i * (sectors) + j + 1;

      int fourth = (i + 1) * (sectors) + (j + 1);

      // first tringle strip
      indices.push_back(first);
      indices.push_back(second);
      indices.push_back(third);

      // second tringle strig
      indices.push_back(second);
      indices.push_back(fourth);
      indices.push_back(third);
    }
  }
  glGenVertexArrays(1, &VAO);
  setupGeometry();
}
