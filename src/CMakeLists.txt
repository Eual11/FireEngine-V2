set(EXECUTABLE_NAME "e")
include_directories($ENV{CPLUS_INCLUDE_PATH} "${CMAKE_SOURCE_DIR}/include")


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_executable(${EXECUTABLE_NAME} ./main.cpp)
add_library(glad STATIC glad.c)
add_library(stb_image stbi_lib.c)
add_library(Shader STATIC Shader.cpp)
add_library(Mesh STATIC EMesh.cpp)
add_library(Model STATIC EModel.cpp)
add_library(Camera STATIC Camera.cpp)
add_library(EWindow STATIC Window.cpp)
add_library(ELight STATIC ELight.cpp)
add_library(EWorld STATIC EWorld.cpp)

target_link_libraries( Model PRIVATE Mesh PRIVATE stb_image PRIVATE Shader PRIVATE glad PRIVATE  "${CMAKE_SOURCE_DIR}\\lib\\assimp-vc143-mt.lib" )
target_link_libraries(EWorld PRIVATE Model PRIVATE ELight)
#  PRIVATE "$ENV{C_LIBRARY_PATH}\\SDL3_mixer.lib"
#glfw, assimp
target_link_libraries(${EXECUTABLE_NAME}  PRIVATE glad PRIVATE Shader PRIVATE stb_image PRIVATE Camera PRIVATE "${CMAKE_SOURCE_DIR}\\lib\\glfw3.lib" PRIVATE Model PRIVATE EWindow PRIVATE EWorld )
set_target_properties(${EXECUTABLE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})




