#ifndef _EOBJECTGEOMETRY_H
#define _EOBJECTGEOMETRY_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <vector>
struct EVertex {
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoord;
};
struct VertexAttrib {
  std::string name;
  GLenum type;
  int size;
  int stride;
  int location;
  size_t offset;
  bool normalize;
};
class EBufferGeometry {

public:
  EBufferGeometry(std::vector<EVertex>, std::vector<unsigned int>);
  EBufferGeometry() = default;

  void addCustomAttrib(VertexAttrib, const std::vector<float> &);
  bool hasIndices();
  size_t getIndiciesCount();
  size_t getVertciesCount();
  void bind();
  void unbind();
  void dispose();

private:
  std::vector<EVertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<VertexAttrib> predefinedAttribs;
  int currentLocation = 0;
  std::unordered_map<unsigned int, VertexAttrib> customAttribs;
  unsigned int VAO, VBO, EBO;
  void setupGeometry();
  void setupPredefinedAttribs();
};
#endif
