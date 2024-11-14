set(EXECUTABLE_NAME "e")
include_directories($ENV{CPLUS_INCLUDE_PATH} "${CMAKE_SOURCE_DIR}/include")


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_executable(${EXECUTABLE_NAME} ./main.cpp)
add_library(glad STATIC glad.c)
add_library(stb_image stbi_lib.c)
add_library(Shader STATIC Shader.cpp)
add_library(Object3D STATIC EObject3D.cpp)
add_library(BufferGeometry STATIC EBufferGeometry.cpp)
add_library(BoxGeometry STATIC EBoxGeometry.cpp)
add_library(QuadGeometry STATIC EQuadGeometry.cpp)
add_library(Material STATIC EMaterial.cpp)
add_library(Texture STATIC ETexture.cpp)
add_library(Mesh STATIC EMesh.cpp)
add_library(Group STATIC EGroup.cpp)
add_library(Model STATIC EModel.cpp)
add_library(Renderer STATIC ERenderer.cpp)
add_library(ModelLoader STATIC EModelLoader.cpp)
add_library(Camera STATIC Camera.cpp)
add_library(EWindow STATIC Window.cpp)
add_library(ELight STATIC ELight.cpp)
add_library(EWorld STATIC EWorld.cpp)
add_library(FrameBuffer STATIC EFrameBuffer.cpp)
target_link_libraries(Camera PRIVATE Object3D)
target_link_libraries(Group PRIVATE Object3D)
target_link_libraries(Material PRIVATE Texture)
target_link_libraries(Mesh PRIVATE BufferGeometry PRIVATE Material)
target_link_libraries(Model PRIVATE Mesh PRIVATE Group  PRIVATE Shader PRIVATE glad  )
target_link_libraries(ModelLoader PRIVATE Texture PRIVATE Mesh PRIVATE Group PRIVATE stb_image PRIVATE Shader PRIVATE glad PRIVATE  "${CMAKE_SOURCE_DIR}\\lib\\assimp-vc143-mt.lib" )
target_link_libraries(EWorld PRIVATE Model PRIVATE ELight)
target_link_libraries(BoxGeometry PRIVATE BufferGeometry)
target_link_libraries(Renderer PRIVATE EWorld PRIVATE Model PRIVATE ELight PRIVATE EWindow PRIVATE FrameBuffer PRIVATE QuadGeometry)
#  PRIVATE "$ENV{C_LIBRARY_PATH}\\SDL3_mixer.lib"
#glfw, assimp
target_link_libraries(${EXECUTABLE_NAME}  PRIVATE glad PRIVATE Shader PRIVATE Texture PRIVATE stb_image PRIVATE Camera PRIVATE Renderer PRIVATE  "${CMAKE_SOURCE_DIR}\\lib\\glfw3.lib" PRIVATE Model PRIVATE EWindow PRIVATE EWorld PRIVATE ModelLoader PRIVATE Texture PRIVATE BoxGeometry )
set_target_properties(${EXECUTABLE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})




