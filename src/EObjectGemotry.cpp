#include "../include/EObjectGeometry.h"

EBufferGeometry::EBufferGeometry(std::vector<EVertex> verts,
                                 std::vector<unsigned int> inds) {
  this->vertices = verts;
  this->indicies = inds;
}

void EBufferGeometry::setupGeometry() {
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(EVertex) * vertices.size(),
               vertices.data(), GL_STATIC_DRAW);

  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indicies.size(),
               indicies.data(), GL_STATIC_DRAW);

  setupPredefinedAttribs();
  /* glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(EVertex), */
  /*                       (void *)offsetof(EVertex, Position)); */
  /* glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(EVertex), */
  /*                       (void *)offsetof(EVertex, Normal)); */
  /* glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(EVertex), */
  /*                       (void *)offsetof(EVertex, TexCoord)); */
  /**/

  for (const auto &d : predefinedAttribs) {
    glEnableVertexAttribArray(d.location);
    glVertexAttribPointer(d.location, d.size, d.type, d.normalize, d.stride,
                          (void *)d.offset);
  }
  /* glEnableVertexAttribArray(0); */
  /* glEnableVertexAttribArray(1); */
  /* glEnableVertexAttribArray(2); */

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void EBufferGeometry::dispose() {
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteVertexArrays(1, &VAO);
}
void EBufferGeometry::setupPredefinedAttribs() {
  predefinedAttribs.push_back({"Position", GL_FLOAT, 3, sizeof(EVertex),
                               currentLocation++, offsetof(EVertex, Position),
                               GL_FALSE});
  predefinedAttribs.push_back({"Normal", GL_FLOAT, 3, sizeof(EVertex),
                               currentLocation++, offsetof(EVertex, Normal),
                               GL_FALSE});
  predefinedAttribs.push_back({"TexCoord", GL_FLOAT, 2, sizeof(EVertex),
                               currentLocation++, offsetof(EVertex, TexCoord),
                               GL_FALSE});
}
void EBufferGeometry::addCustomAttrib(VertexAttrib attrib,
                                      const std::vector<float> &data) {
  glBindVertexArray(VAO);
  unsigned int bufferID;
  glGenBuffers(1, &bufferID);
  glBindBuffer(GL_ARRAY_BUFFER, bufferID);

  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), data.data(),
               GL_STATIC_DRAW);

  glVertexAttribPointer(currentLocation, attrib.size, attrib.type,
                        attrib.normalize, attrib.size, (void *)attrib.offset);

  // maybe enable it later?

  glEnableVertexAttribArray(currentLocation++);
  customAttribs[bufferID] = attrib;
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
