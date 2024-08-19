#ifndef _E_MESH_H
#define _E_MESH_H

#include "Shader.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

struct ETexture {
  unsigned int ID = 0;
  std::string type;
  std::string path;
};

struct EVertex {

  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoord;
};
class EMesh {

public:
  EMesh(std::vector<EVertex>, std::vector<ETexture>, std::vector<unsigned int>);

  ~EMesh();
  void Draw(Shader &);

private:
  std::vector<EVertex> vertices;
  std::vector<ETexture> textures;
  std::vector<unsigned int> indicies;

  unsigned int VAO, VBO, EBO;
  void setupMesh();
};

#endif
