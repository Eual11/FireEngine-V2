#ifndef SHADER_H
#define SHADER_H
#include <fstream>
#include <glad/glad.h>
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

  void Use();
  void setInt(std::string &name, int val) const;
  void setBool(std::string &name, bool val) const;
  void setFloat(std::string name, float val) const;
  void setDouble(std::string &name, float val) const;
};
#endif
