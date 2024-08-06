#include "../include/Mesh.h"
#include <string>
Mesh::Mesh(std::vector<Vertex> v, std::vector<Texture> t,
           std::vector<unsigned int> id) {
  this->vertices = v;
  this->textures = t;
  this->indices = id;
  setupMesh();
}

void Mesh::setupMesh() {
  // creating VAO
  glGenVertexArrays(1, &VAO);
  GL_CHECK(glBindVertexArray(VAO));
  // Creating VBO, EBO and binding them

  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0],
               GL_STATIC_DRAW);
  GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                        sizeof(unsigned int) * indices.size(), &indices[0],
                        GL_STATIC_DRAW));
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, Position));
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)(offsetof(Vertex, Normal)));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)(offsetof(Vertex, TexCoord)));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glBindVertexArray(0);
}
void Mesh::Draw(Shader &shader) {

  unsigned int numDiffuse = 1;
  unsigned int numSpecular = 1;

  glBindVertexArray(VAO);
  shader.Use();

  for (size_t i = 0; i < textures.size(); i++) {
    Texture tex = textures[i];
    std::string tex_name = "";
    GL_CHECK(glActiveTexture(GL_TEXTURE0 + i));
    if (tex.type == "texture_diffuse") {
      tex_name = tex.type + std::to_string(numDiffuse++);
    } else if (tex.type == "texture_specular") {
      tex_name = tex.type + std::to_string(numSpecular++);
    }
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, tex.ID));
    shader.setInt("material." + tex_name, i);
  }

  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(0);
}
