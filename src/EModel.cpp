#include "../include/EModel.h"
#include <assimp/material.h>
#include <assimp/types.h>
#include <filesystem>
#include <glm/trigonometric.hpp>
#include <memory>
EModel::EModel(std::string path) { loadModel(path); }

void EModel::render(Shader &shader) {
  shader.setMat4("uModel", getWorldMatrix());
  for (auto &mesh : meshes) {
    mesh.render(shader);
  }
}

void EModel::loadModel(std::string path) {
  Assimp::Importer importer;

  const aiScene *scene =
      importer.ReadFile(path, aiProcess_GenNormals | aiProcess_Triangulate);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {

    fprintf(stderr, "Failed to Load Model");
    return;
  }

  std::filesystem::path p(path);
  root_path = std::filesystem::absolute(p.parent_path().string()).string();
  ProcessNode(glm::mat4(1.0f), scene->mRootNode, scene);
}
void EModel::ProcessNode(glm::mat4 m, aiNode *node, const aiScene *scene) {

  aiMatrix4x4 nm = node->mTransformation;
  glm::mat4 node_transform = {
      nm.a1, nm.b1, nm.c1, nm.d1, nm.a2, nm.b2, nm.c2, nm.d2,
      nm.a3, nm.b3, nm.c3, nm.d3, nm.a4, nm.b4, nm.c4, nm.d4,
  };

  m = node_transform * m;

  for (int i = 0; i < (int)node->mNumMeshes; i++) {
    meshes.push_back(ProcessMesh(m, scene->mMeshes[node->mMeshes[i]], scene));
  }

  for (int i = 0; i < (int)node->mNumChildren; i++) {
    ProcessNode(m, node->mChildren[i], scene);
  }
}
EMesh EModel::ProcessMesh(glm::mat4 &m, aiMesh *mesh, const aiScene *scene) {

  std::vector<EVertex> vertices;
  std::vector<ETexture> textures;
  std::vector<unsigned int> indicies;

  glm::mat4 m_ive = m;
  // Loading vertices
  for (int i = 0; i < (int)mesh->mNumVertices; i++) {

    EVertex v;
    glm::vec4 pos(mesh->mVertices[i].x, mesh->mVertices[i].y,
                  mesh->mVertices[i].z, 1.0f);

    v.Position = m_ive * pos;

    if (mesh->mTextureCoords[0]) {
      v.TexCoord.x = mesh->mTextureCoords[0][i].x;
      v.TexCoord.y = mesh->mTextureCoords[0][i].y;
    } else
      v.TexCoord = {0, 0};
    if (mesh->HasNormals()) {
      v.Normal = glm::mat3(glm::transpose(glm::inverse(m_ive))) *
                 glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y,
                           mesh->mNormals[i].z);
    }

    vertices.push_back(v);
  }
  std::shared_ptr<Material> meshMaterial = nullptr;
  // loading materials
  if (mesh->mMaterialIndex >= 0) {

    // TODO: IMPLEMENT A WAY TO SELECT A MATERIAL BASED ON THE SHADING MODEL
    // USED
    auto phongMeshMaterial = std::make_shared<PhongMaterial>(PhongMaterial());

    float val = 0.0f;
    aiColor3D color;
    aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];

    if (mat->Get(AI_MATKEY_SHININESS, val) == AI_SUCCESS) {
      phongMeshMaterial->shininess = val;
    }
    if (mat->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS) {
      phongMeshMaterial->diffuse_color = glm::vec3(color.r, color.g, color.b);
    }
    if (mat->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS) {
      phongMeshMaterial->ambient_color = glm::vec3(color.r, color.g, color.b);
    }

    if (mat->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS) {
      phongMeshMaterial->specular_color = glm::vec3(color.r, color.g, color.b);
    }
    std::vector<ETexture> diffuseTextures =
        loadMaterialTexture(mat, aiTextureType_DIFFUSE, "texture_diffuse");

    /* printf("Speculars has %d diffuse textures\n", diffuseTextures.size()); */
    phongMeshMaterial->textures.insert(phongMeshMaterial->textures.end(),
                                       diffuseTextures.begin(),
                                       diffuseTextures.end());

    std::vector<ETexture> specularTextures =
        loadMaterialTexture(mat, aiTextureType_SPECULAR, "texture_specular");
    phongMeshMaterial->textures.insert(phongMeshMaterial->textures.end(),
                                       specularTextures.begin(),
                                       specularTextures.end());
    /* printf("Speculars has %d specular textures\n", specularTextures.size());
     */
    meshMaterial = phongMeshMaterial;
  }

  for (int i = 0; i < (int)mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];

    for (int j = 0; j < (int)face.mNumIndices; j++) {
      indicies.push_back(face.mIndices[j]);
    }
  }

  return EMesh(
      std::make_shared<EBufferGeometry>(EBufferGeometry(vertices, indicies)),
      meshMaterial);
}

std::vector<ETexture> EModel::loadMaterialTexture(aiMaterial *mat,
                                                  aiTextureType type,
                                                  std::string type_str) {

  std::vector<ETexture> textures;

  for (int i = 0; i < (int)mat->GetTextureCount(type); i++) {

    ETexture tex;
    aiString tex_path;
    mat->GetTexture(type, i, &tex_path);

    tex.type = type_str;
    tex.path = tex_path.C_Str();

    if (loaded_texture.find(tex.path) != loaded_texture.end()) {
      tex.ID = loaded_texture[tex.path];

    } else {
      // if it's not found
      tex.ID = TextureFromFile(tex.path);
      if (tex.ID != 0)
        loaded_texture[tex.path] = tex.ID;
    }
    textures.push_back(tex);
  }

  return textures;
}
unsigned int EModel::TextureFromFile(std::string path) {
  unsigned int texID = 0;

  std::string filename = "";

#ifdef _WIN32
  filename = root_path + "\\" + path;
#else
  filename = dir + "/" + path;
#endif

  if (std::filesystem::exists(filename)) {

  } else {
    fprintf(stderr, "Filename %s doesn't exist, Loading Default Textures\n",
            filename.c_str());
    filename = "../models/textures/3DLABbg_UV_Map_Checker_01_2048x2048.jpg";
  }

  unsigned char *buf = nullptr;

  int w, h, nchr;
  glGenTextures(1, &texID);

  stbi_set_flip_vertically_on_load(true);
  buf = stbi_load(filename.c_str(), &w, &h, &nchr, 0);

  if (buf) {
    glBindTexture(GL_TEXTURE_2D, texID);
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
                 buf);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    fprintf(stderr, "Unable to load Texture check path: %s\n",
            filename.c_str());
    texID = 0;
  }

  glBindTexture(GL_TEXTURE_2D, 0);
  stbi_image_free(buf);
  return texID;
}
