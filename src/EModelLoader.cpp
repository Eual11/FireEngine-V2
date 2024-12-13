#include "../include/EModelLoader.h"
#include <memory>

std::shared_ptr<EModel>
EModelLoader::loadModel(std::string path, std::shared_ptr<Material> material) {
  auto model = std::make_shared<EModel>();

  Assimp::Importer importer;

  const aiScene *scene =
      importer.ReadFile(path, aiProcess_GenNormals | aiProcess_Triangulate);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode || !model) {
    fprintf(stderr, "Failed to Load Model");
    return model;
  }

  std::filesystem::path p(path);
  root_path = std::filesystem::absolute(p.parent_path()).string();

  try {
    // naming model
    model->setName(scene->mRootNode->mName.C_Str());
    model->add(ProcessNode(glm::mat4(1.0f), scene->mRootNode, scene));
  } catch (std::bad_weak_ptr &) {
    printf("BAD PTR\n!!!");
  }

  // settign material to overide the model's material
  if (material) {
    model->setMaterial(material);
  }
  return model;
}
std::shared_ptr<EObject3D> EModelLoader::ProcessNode(glm::mat4 m, aiNode *node,
                                                     const aiScene *scene) {

  /* This is shit */
  aiMatrix4x4 nm = node->mTransformation;
  glm::mat4 node_transform = {
      nm.a1, nm.b1, nm.c1, nm.d1, nm.a2, nm.b2, nm.c2, nm.d2,
      nm.a3, nm.b3, nm.c3, nm.d3, nm.a4, nm.b4, nm.c4, nm.d4,
  };

  m = node_transform * m;

  auto currentNode = std::make_shared<EGroup>();
  for (int i = 0; i < (int)node->mNumMeshes; i++) {
    if (currentNode) {

      auto nMesh = ProcessMesh(m, scene->mMeshes[node->mMeshes[i]], scene);
      // naming mesh
      nMesh->setName(scene->mMeshes[node->mMeshes[i]]->mName.C_Str());
      currentNode->add(nMesh);
    }
  }
  currentNode->setModelMatrix(node_transform);

  for (int i = 0; i < (int)node->mNumChildren; i++) {
    if (currentNode)
      currentNode->add(ProcessNode(m, node->mChildren[i], scene));
  }
  return currentNode;
}
std::shared_ptr<EMesh> EModelLoader::ProcessMesh(glm::mat4 &m, aiMesh *mesh,
                                                 const aiScene *scene) {

  std::vector<EVertex> vertices;
  std::vector<ETexture> textures;
  std::vector<unsigned int> indicies;

  // Loading vertices
  for (int i = 0; i < (int)mesh->mNumVertices; i++) {

    EVertex v;
    glm::vec4 pos(mesh->mVertices[i].x, mesh->mVertices[i].y,
                  mesh->mVertices[i].z, 1.0f);

    // NOTE: Removing the matrix multiplcation to test things
    v.Position = pos;

    if (mesh->mTextureCoords[0]) {
      v.TexCoord.x = mesh->mTextureCoords[0][i].x;
      v.TexCoord.y = mesh->mTextureCoords[0][i].y;
    } else
      v.TexCoord = {0, 0};
    if (mesh->HasNormals()) {
      v.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y,
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

    phongMeshMaterial->textures.insert(phongMeshMaterial->textures.end(),
                                       diffuseTextures.begin(),
                                       diffuseTextures.end());

    std::vector<ETexture> specularTextures =
        loadMaterialTexture(mat, aiTextureType_SPECULAR, "texture_specular");
    phongMeshMaterial->textures.insert(phongMeshMaterial->textures.end(),
                                       specularTextures.begin(),
                                       specularTextures.end());
    meshMaterial = phongMeshMaterial;
  }

  for (int i = 0; i < (int)mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];

    for (int j = 0; j < (int)face.mNumIndices; j++) {
      indicies.push_back(face.mIndices[j]);
    }
  }
  auto geometry = std::make_shared<EBufferGeometry>(vertices, indicies);
  if (!geometry) {
    std::cerr << "Failed to create geometry!" << std::endl;
  }

  auto nMesh = std::make_shared<EMesh>(geometry, meshMaterial);
  if (!nMesh) {
    std::cerr << "Failed to create mesh!" << std::endl;
  }
  return nMesh;
}

std::vector<ETexture> EModelLoader::loadMaterialTexture(aiMaterial *mat,
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
unsigned int EModelLoader::TextureFromFile(std::string path) {
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
