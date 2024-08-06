#include "../include/Model.h"
#include <assimp/postprocess.h>
#include <filesystem>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>
Model::Model(std::string path) { loadModel(path); }
void Model::Draw(Shader &shader) {
  /* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); */
  for (auto &mesh : meshes) {
    mesh.Draw(shader);
  }
}

void printMat4(const glm::mat4 &mat, float epsilon = 1e-5f) {
  const float *pMat = glm::value_ptr(mat);
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      float value = pMat[j + i * 4];
      if (fabs(value) < epsilon) {
        value = 0.0f;
      }
      std::cout << value << " ";
    }
    std::cout << std::endl;
  }
}

bool Model::loadModel(std::string &_path) {

  Assimp::Importer importer;
  const aiScene *scene =
      importer.ReadFile(_path, aiProcess_Triangulate | aiProcess_GenNormals);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    fprintf(stderr, "Failed To load model: %s\n", importer.GetErrorString());
    return false;
  }
  std::filesystem::path p(_path);
  dir = std::filesystem::absolute(p.parent_path()).string();
  processNode(glm::mat4(1.0f), scene->mRootNode, scene);
  return true;
}

void Model::processNode(glm::mat4 mat, aiNode *node, const aiScene *scene) {
  // process all meshes of the current Node
  //
  aiMatrix4x4 m = node->mTransformation;
  glm::mat4 node_transform = {

      m.a1, m.b1, m.c1, m.d1, m.a2, m.b2, m.c2, m.d2,
      m.a3, m.b3, m.c3, m.d3, m.a4, m.b4, m.c4, m.d4,
  };
  mat = node_transform * mat;
  for (size_t i = 0; i < node->mNumMeshes; i++) {

    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(processMesh(mat, mesh, scene));
  }

  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    processNode(mat, node->mChildren[i], scene);
  }
}
Mesh Model::processMesh(glm::mat4 &m, aiMesh *mesh, const aiScene *scene) {
  std::vector<Vertex> vertices;
  std::vector<Texture> textures;
  std::vector<unsigned int> indicies;

  // processing vertices
  glm::mat4 m_ive = m; // glm::inverse(m);
  for (size_t i = 0; i < mesh->mNumVertices; i++) {
    Vertex v;

    glm::vec4 pos(mesh->mVertices[i].x, mesh->mVertices[i].y,
                  mesh->mVertices[i].z, 1.0f);
    v.Position = m_ive * pos;

    if (mesh->HasNormals()) {
      glm::vec3 normal(mesh->mNormals[i].x, mesh->mNormals[i].y,
                       mesh->mNormals[i].z);
      v.Normal = glm::mat3(m_ive) * normal;
      v.Normal = glm::transpose(glm::mat3(glm::inverse(m_ive))) * normal;
    }

    if (mesh->mTextureCoords[0]) {
      v.TexCoord.x = mesh->mTextureCoords[0][i].x;
      v.TexCoord.y = mesh->mTextureCoords[0][i].y;
    } else
      v.TexCoord = glm::vec2(0.0f, 0.0f);

    vertices.push_back(v);
  }

  // material...

  // if the mesh has a material index

  if (mesh->mMaterialIndex >= 0) {

    aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<Texture> diffuse_textures =
        loadMaterialTexture(mat, aiTextureType_DIFFUSE, "texture_diffuse");

    // appending diffuse textures
    textures.insert(textures.end(), diffuse_textures.begin(),
                    diffuse_textures.end());

    // appending specular textures

    std::vector<Texture> specular_textures =
        loadMaterialTexture(mat, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specular_textures.begin(),
                    specular_textures.end());
  }
  //  indicies

  for (size_t i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (size_t j = 0; j < face.mNumIndices; j++)
      indicies.push_back(face.mIndices[j]);
  }

  return Mesh(vertices, textures, indicies);
}
std::vector<Texture> Model::loadMaterialTexture(aiMaterial *mat,
                                                aiTextureType type,
                                                std::string type_str) {

  std::vector<Texture> textures;

  for (size_t i = 0; i < mat->GetTextureCount(type); i++) {
    Texture tex;
    aiString str;
    mat->GetTexture(type, i, &str);

    tex.type = type_str;
    tex.path = str.C_Str();
    if (loaded_textures.find(tex.path) != loaded_textures.end()) {
      textures.push_back(loaded_textures[tex.path]);
    } else {
      tex.ID = textureFromFile(str.C_Str(), dir);
      loaded_textures[tex.path] = tex;
      textures.push_back(tex);
    }
  }

  return textures;
}

unsigned int Model::textureFromFile(std::string path, std::string dir) {
  unsigned int textureID;
#ifdef _WIN32
  std::string filename = dir + "\\" + path;
#else

  std::string filename = dir + "/" + path;
#endif
  if (std::filesystem::exists(filename)) {
    filename = std::filesystem::canonical(filename).string();
  } else {
    printf("filename %s doesn't exist, loading default texture\n",
           filename.c_str());
    filename = "../models/textures/3DLABbg_UV_Map_Checker_01_2048x2048.jpg";
  }

  glGenTextures(1, &textureID);

  unsigned char *buffer = nullptr;

  int w, h, nchr;
  buffer = stbi_load(filename.c_str(), &w, &h, &nchr, 0);

  if (buffer) {
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    unsigned int format;

    switch (nchr) {
    case 1: {
      format = GL_RED;
      break;
    }
    case 3: {
      format = GL_RGB;
      break;
    }
    case 4: {
      format = GL_RGBA;
      break;
    }
    }
    glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE,
                 buffer);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    printf("Unable to load Texture check path %s\n", filename.c_str());
  }
  stbi_image_free(buffer);
  return textureID;
}
