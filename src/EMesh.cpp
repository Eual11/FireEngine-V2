#include "../include/EMesh.h"
#include <cstddef>
EMesh::EMesh(std::vector<EVertex> verts, std::vector<ETexture> texs,
             std::vector<unsigned int> indx) {

  this->vertices = verts;
  this->textures = texs;
  this->indicies = indx;

  setupMesh();
}
EMesh::~EMesh() {
  /* glDeleteVertexArrays(1, &VAO); */
  /* glDeleteBuffers(1, &VBO); */
  /* glDeleteBuffers(1, &EBO); */
}

void EMesh::setupMesh() {

  glGenVertexArrays(1, &VAO);
  GL_CHECK(glBindVertexArray(VAO));
  // Creating VBO, EBO and binding them

  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(EVertex) * vertices.size(),
               vertices.data(), GL_STATIC_DRAW);
  GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                        sizeof(unsigned int) * indicies.size(), indicies.data(),
                        GL_STATIC_DRAW));
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(EVertex),
                        (void *)offsetof(EVertex, Position));
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(EVertex),
                        (void *)(offsetof(EVertex, Normal)));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(EVertex),
                        (void *)(offsetof(EVertex, TexCoord)));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glBindVertexArray(0);
}

void EMesh::Draw(Shader &shader) {

  unsigned int numDiffuse = 1;
  unsigned int numSpecular = 1;

  glBindVertexArray(VAO);

  shader.Use();

  for (size_t i = 0; i < textures.size(); i++) {
    ETexture tex = textures[i];
    std::string tex_name = "";
    glActiveTexture(GL_TEXTURE0 + i);
    if (tex.type == "texture_diffuse") {
      tex_name = tex.type + std::to_string(numDiffuse++);
    } else if (tex.type == "texture_specular") {
      tex_name = tex.type + std::to_string(numSpecular++);
    }
    glBindTexture(GL_TEXTURE_2D, tex.ID);
    shader.setInt("material." + tex_name, i);
  }

  glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, 0);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(0);
}
