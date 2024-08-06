#ifndef _ERROR_H
#define _ERROR_H
/* #define LOG ; */
#include <glad/glad.h>
#include <iostream>
#include <string>
inline void checkOpenGLError(const char *stmt, const char *fname, int line) {
  GLenum err = glGetError();
  while (err != GL_NO_ERROR) {
    std::string error;
    switch (err) {
    case GL_INVALID_OPERATION:
      error = "INVALID_OPERATION";
      break;
    case GL_INVALID_ENUM:
      error = "INVALID_ENUM";
      break;
    case GL_INVALID_VALUE:
      error = "INVALID_VALUE";
      break;
    case GL_OUT_OF_MEMORY:
      error = "OUT_OF_MEMORY";
      break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
      error = "INVALID_FRAMEBUFFER_OPERATION";
      break;
    default:
      error = "Unknown Error";
      break;
    }
#ifdef LOG
    std::cerr << "GL_" << error << " - " << fname << ":" << line << " - "
              << stmt << std::endl;
#endif
    err = glGetError();
  }
}

#define GL_CHECK(stmt)                                                         \
  do {                                                                         \
    stmt;                                                                      \
    checkOpenGLError(#stmt, __FILE__, __LINE__);                               \
  } while (0)

#endif
