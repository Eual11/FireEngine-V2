#ifndef _EOBJECTGEOMETRY_H
#define _EOBJECTGEOMETRY_H

#include <glad/glad.h>
#include <glm/ext/scalar_constants.hpp>
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
  GLenum type = GL_FLOAT;
  int size;
  int stride;
  int location;
  size_t offset = 0;
  bool normalize = GL_FALSE;
};
class EBufferGeometry {

public:
  EBufferGeometry(std::vector<EVertex>, std::vector<unsigned int>);
  EBufferGeometry();

  void addCustomAttribF(VertexAttrib, const float *, size_t size);
  bool hasIndices();
  size_t getIndiciesCount();
  size_t getVertciesCount();
  void bind();
  void unbind();
  void dispose();

protected:
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
