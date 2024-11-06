#include "../include/EBoxGeometry.h"

EBoxGeometry::EBoxGeometry(float width, float height, float depth) {
  float w = width / 2, h = height / 2, d = depth / 2;
  vertices = {
      // Front face
      {{-w, -h, d}, {0, 0, 1}, {0, 0}}, // front bottom-left
      {{w, -h, d}, {0, 0, 1}, {1, 0}},  // front bottom-right
      {{w, h, d}, {0, 0, 1}, {1, 1}},   // front top-right
      {{-w, h, d}, {0, 0, 1}, {0, 1}},  // front top-left

      // Back face
      {{-w, -h, -d}, {0, 0, -1}, {1, 0}}, // back bottom-left
      {{w, -h, -d}, {0, 0, -1}, {0, 0}},  // back bottom-right
      {{w, h, -d}, {0, 0, -1}, {0, 1}},   // back top-right
      {{-w, h, -d}, {0, 0, -1}, {1, 1}},  // back top-left

      // Left face
      {{-w, -h, -d}, {-1, 0, 0}, {0, 0}}, // left bottom-back
      {{-w, -h, d}, {-1, 0, 0}, {1, 0}},  // left bottom-front
      {{-w, h, d}, {-1, 0, 0}, {1, 1}},   // left top-front
      {{-w, h, -d}, {-1, 0, 0}, {0, 1}},  // left top-back

      // Right face
      {{w, -h, -d}, {1, 0, 0}, {1, 0}}, // right bottom-back
      {{w, -h, d}, {1, 0, 0}, {0, 0}},  // right bottom-front
      {{w, h, d}, {1, 0, 0}, {0, 1}},   // right top-front
      {{w, h, -d}, {1, 0, 0}, {1, 1}},  // right top-back

      // Top face
      {{-w, h, d}, {0, 1, 0}, {0, 0}},  // top front-left
      {{w, h, d}, {0, 1, 0}, {1, 0}},   // top front-right
      {{w, h, -d}, {0, 1, 0}, {1, 1}},  // top back-right
      {{-w, h, -d}, {0, 1, 0}, {0, 1}}, // top back-left

      // Bottom face
      {{-w, -h, d}, {0, -1, 0}, {1, 0}}, // bottom front-left
      {{w, -h, d}, {0, -1, 0}, {0, 0}},  // bottom front-right
      {{w, -h, -d}, {0, -1, 0}, {0, 1}}, // bottom back-right
      {{-w, -h, -d}, {0, -1, 0}, {1, 1}} // bottom back-left
  };

  indices = {// Front face
             0, 1, 2, 2, 3, 0,

             // Back face
             4, 5, 6, 6, 7, 4,

             // Left face
             8, 9, 10, 10, 11, 8,

             // Right face
             12, 13, 14, 14, 15, 12,

             // Top face
             16, 17, 18, 18, 19, 16,

             // Bottom face
             20, 21, 22, 22, 23, 20};

  glGenVertexArrays(1, &VAO);
  setupGeometry();
}
