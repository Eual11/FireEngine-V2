#ifndef SHADER_H
#define SHADER_H
#include "../include/Error.h"
#include <fstream>
#include <glad/glad.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>

class Shader {

public:
  unsigned int ID;
  unsigned int vertexShader;
  unsigned int fragmentShader;

  Shader(const char *vertSrc, const char *fragSrc);

  void Use() const;
  void setInt(std::string name, int val) const;
  void setBool(std::string name, bool val) const;
  void setFloat(std::string name, float val) const;
  void setDouble(std::string name, float val) const;
  void setVec3(std::string name, glm::vec3 v);
  void setVec4(std::string name, glm::vec4 v);

  void setMat4(std::string name, glm::mat4 m);
  void setMat3(std::string name, glm::mat3);

  void setVec3(std::string name, float x, float y, float z);
  void setVec4(std::string name, float x, float y, float z, float w);
};
#endif
