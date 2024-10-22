#include "stb_image.h"
#include <filesystem>
#include <glad/glad.h>
#include <memory>
#include <string>
#include <unordered_map>

struct ETexture {

  static std::unordered_map<std::string, std::shared_ptr<ETexture>>
      texture_cache;
  static std::shared_ptr<ETexture> load(std::string path,
                                        std::string root_path = "");
  enum ETexture_Wraping {

    REPEAT = GL_REPEAT,
    MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
    CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
    CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,

  };
  enum ETexture_Filter {
    LINEAR = GL_LINEAR,
    NEAREST = GL_NEAREST,
    NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
    LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
    LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
    NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR
  };

  ETexture() = default;

  unsigned int wrap_s = GL_REPEAT;
  unsigned int wrap_t = GL_REPEAT;
  unsigned int mag_filter = GL_LINEAR;
  unsigned int min_filter = GL_LINEAR_MIPMAP_LINEAR;
  unsigned int ID = 0;
  std::string type = "";
  std::string path = "";
  void setWraping(ETexture_Wraping s, ETexture_Wraping t);
  void setFilter(ETexture_Filter min_filter, ETexture_Filter mag_filter);
};
ETexture TextureFromFile(std::string path, std::string root_path = "");
