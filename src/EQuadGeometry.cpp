#include "../include/EQuadGeometry.h"

EQuadGeometry::EQuadGeometry(float w, float h) {
  // Define vertices in NDC to cover the whole screen
  vertices = {
      {{-1.0f, -1.0f, 0.0f}, {0, 0, 1}, {0, 0}}, // bottom-left
      {{1.0f, -1.0f, 0.0f}, {0, 0, 1}, {1, 0}},  // bottom-right
      {{1.0f, 1.0f, 0.0f}, {0, 0, 1}, {1, 1}},   // top-right
      {{-1.0f, 1.0f, 0.0f}, {0, 0, 1}, {0, 1}}   // top-left
  };
  this->name = "Quad";

  indices = {
      0, 1, 2, // first triangle
      2, 3, 0  // second triangle
  };

  // OpenGL setup
  glGenVertexArrays(1, &VAO);
  setupGeometry();
}
