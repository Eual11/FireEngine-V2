#include "../include/ETexture.h"
#include <memory>
void ETexture::setWraping(ETexture_Wraping s, ETexture_Wraping t) {
  glBindTexture(GL_TEXTURE_2D, ID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t);
  glBindTexture(GL_TEXTURE_2D, 0);
}
void ETexture::setFilter(ETexture_Filter min_filter,
                         ETexture_Filter mag_filter) {
  glBindTexture(GL_TEXTURE_2D, ID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
  glBindTexture(GL_TEXTURE_2D, 0);
}

std::shared_ptr<ETexture> ETexture::load(std::string path,
                                         std::string root_path) {
  std::unordered_map<std::string, std::shared_ptr<ETexture>> texture_cache;
  std::string filename = path;

  if (root_path != "") {
#ifdef _WIN32
    filename = root_path + "\\" + path;
#else
    filename = dir + "/" + path;
#endif
  }

  if (std::filesystem::exists(filename)) {

  } else {
    fprintf(stderr, "Filename %s doesn't exist, Loading Default Textures\n",
            filename.c_str());
    filename = "../models/textures/3DLABbg_UV_Map_Checker_01_2048x2048.jpg";
  }
  if (texture_cache.find(filename) != texture_cache.end()) {
    return texture_cache[filename];
  }

  printf("%s\n", filename.c_str());
  auto new_tex = std::make_shared<ETexture>(ETexture());
  unsigned char *buf = nullptr;

  new_tex->path = path;
  int w, h, nchr;
  glGenTextures(1, &new_tex->ID);

  stbi_set_flip_vertically_on_load(true);
  buf = stbi_load(filename.c_str(), &w, &h, &nchr, 0);

  if (buf) {
    glBindTexture(GL_TEXTURE_2D, new_tex->ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, new_tex->wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, new_tex->wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, new_tex->mag_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, new_tex->wrap_s);
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
    new_tex->ID = 0;
    new_tex->path = "";
  }
  glBindTexture(GL_TEXTURE_2D, 0);
  stbi_image_free(buf);
  stbi_set_flip_vertically_on_load(false);
  texture_cache[filename] = new_tex;
  return new_tex;
}
