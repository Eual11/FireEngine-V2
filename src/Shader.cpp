#include "../include/Shader.h"
#include <gl/gl.h>
#include <glm/gtc/type_ptr.hpp>
using glm::value_ptr;

Shader::Shader(const char *vertSrc, const char *fragSrc) {
  std::string vertexShaderSrc;
  std::string fragmentShaderSrc;
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;
  std::stringstream vShaderStream;
  std::stringstream fShaderStream;

  vShaderFile.exceptions(std::ios::failbit | std::ios::badbit);
  fShaderFile.exceptions(std::ios::failbit | std::ios::badbit);

  try {

    vShaderFile.open(vertSrc);
    fShaderFile.open(fragSrc);

    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    vertexShaderSrc = vShaderStream.str();
    fragmentShaderSrc = fShaderStream.str();

    vShaderFile.close();
    fShaderFile.close();
  } catch (std::ifstream::failure e) {

    std::cerr << "Failed to Open Shaders\n"
              << "Check the Path: " << vertSrc << "  " << fragSrc << std::endl;
  }
  const char *vertexShaderCode = vertexShaderSrc.c_str();
  const char *fragmentShaderCode = fragmentShaderSrc.c_str();

  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  ID = glCreateProgram();
  glShaderSource(vertexShader, 1, &vertexShaderCode, nullptr);
  glShaderSource(fragmentShader, 1, &fragmentShaderCode, nullptr);
  // compiling shaders
  glCompileShader(vertexShader);
  glCompileShader(fragmentShader);

  int sucess;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &sucess);

  if (!sucess) {

    char infoLog[512] = {'\0'};

    fprintf(stderr, "Failed to compile vertex shader\n");
    glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
    fprintf(stderr, "ERROR: %s", infoLog);
  }
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &sucess);

  if (!sucess) {
    char infoLog[512] = {'\0'};
    fprintf(stderr, "Failed to compile fragmentShader shader\n");
    glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
    fprintf(stderr, "ERROR: %s", infoLog);
  }

  glAttachShader(ID, vertexShader);
  glAttachShader(ID, fragmentShader);
  glLinkProgram(ID);
  sucess = 0;
  glGetProgramiv(ID, GL_LINK_STATUS, &sucess);
  if (!sucess) {

    char infoLog[512] = {'\0'};

    fprintf(stderr, "Failed to Linkshader\n");
    glGetProgramInfoLog(ID, 512, nullptr, infoLog);
    fprintf(stderr, "Error: %s\n", infoLog);
  }
}

void Shader::Use() const { glUseProgram(ID); }
void Shader::setInt(std::string name, int val) const {
  Use();
  GL_CHECK(glUniform1i(glGetUniformLocation(ID, name.c_str()), val));
}
void Shader::setBool(std::string name, bool val) const {
  Use();
  GL_CHECK(glUniform1i(glGetUniformLocation(ID, name.c_str()), val));
}
void Shader::setFloat(std::string name, float val) const {
  Use();
  GL_CHECK(glUniform1f(glGetUniformLocation(ID, name.c_str()), val));
}

void Shader::setVec3(std::string name, glm::vec3 v) {
  Use();
  GL_CHECK(
      glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, value_ptr(v)));
}
void Shader::setVec4(std::string name, glm::vec4 v) {
  Use();
  GL_CHECK(
      glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, value_ptr(v)));
}
void Shader::setMat4(std::string name, glm::mat4 m) {
  Use();
  GL_CHECK(glUniformMatrix4fv((glGetUniformLocation(ID, name.c_str())), 1,
                              GL_FALSE, glm::value_ptr(m)));
}
void Shader::setMat3(std::string name, glm::mat3 m) {
  Use();
  GL_CHECK(glUniformMatrix4fv((glGetUniformLocation(ID, name.c_str())), 1,
                              GL_FALSE, glm::value_ptr(m)));
}
