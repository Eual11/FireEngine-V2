#ifndef __MESH_H
#define __MESH_H

#include "Shader.h"
#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <string>
#include <vector>

struct Vertex {
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoord;
};

struct Texture {
  unsigned int ID = 0;
  std::string type;
  std::string path;
};

class Mesh {

public:
  std::vector<Vertex> vertices;
  std::vector<Texture> textures;
  std::vector<unsigned int> indices;

  Mesh(std::vector<Vertex>, std::vector<Texture>, std::vector<unsigned int>);

  void Draw(Shader &shader);

private:
  unsigned int VAO, VBO, EBO;
  void setupMesh();
};

#endif
