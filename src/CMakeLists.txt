CPMAddPackage(
  NAME glfw 
  GITHUB_REPOSITORY glfw/glfw
  GIT_TAG origin/master
  )

set(EXECUTABLE_NAME "e")
include_directories($ENV{CPLUS_INCLUDE_PATH} "${CMAKE_SOURCE_DIR}/include")


add_executable(${EXECUTABLE_NAME} ./main.cpp)
add_library(glad STATIC glad.c)
add_library(Shader STATIC Shader.cpp)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE glfw PRIVATE glad PRIVATE Shader)
set_target_properties(${EXECUTABLE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})




