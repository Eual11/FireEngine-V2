#ifndef __WINDOW__H
#define __WINDOW__H

#include "../include/Camera.h"
#include "../include/Shader.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <memory>
#include <string>

constexpr int DEFAULT_WIDTH = 800;
constexpr int DEFAULT_HEIGHT = 600;

struct ImageSize {
  int w = 0;
  int h = 0;
};
class Window {
private:
  GLFWwindow *window_ = nullptr;
  ImageSize size_;
  std::string title_;

  // default
  bool keyInputEnabled_ = false;
  bool mouseInputEnabled_ = false;
  bool scollInputEnabled_ = false;
  bool cursorEnabled_ = true;
  bool depthTestingEnabled_ = false;
  bool fullscreen_ = false;
  bool resizeUpdatesEnabled_ = false;
  bool cameraBound = false;
  bool isOpen_ = false;
  float lastFrameTime_ = 0.0f;
  float cursorPosX = 0.0f;
  float cursorPosY = 0.0f;
  bool firstMove = true;
  float deltaTime = 0.0f;

  std::vector<std::function<void(int, int)>> registeredResizeCallbacks;

  std::shared_ptr<Camera> camera_ = nullptr;

public:
  Window(int w, int h, std::string title, bool fullscreen);
  ~Window();
  void DestroyWindow();
  ImageSize getSize() { return size_; }

  float GetAspectRatio();
  std::shared_ptr<Camera> getBoundCamera() const { return camera_; }
  GLFWwindow *GetRef();
  void SetContextVersion(unsigned int major, unsigned int minor);
  // activating the window by setting it as the current opengl context
  void Activate();
  void SetViewport();

  void EnableCursor();
  void EnableVsync();
  void EnableKeyInput();
  void EnableScrollInput();
  void EnableResizing();
  void EnableMouseInput();

  // TODO: Implement the disable functions
  void DisableMouseInput();
  void DisableResizing();
  void DisableDepthTesting();
  void DisableScrollInput();
  void DisableKeyInput();
  void DisableCursor();
  void DisableVsync();
  void Update();
  void UpdateUniforms(Shader &shader);

  // bindings
  //
  //
  void BindCamera(std::shared_ptr<Camera>);
  // setting callbacks

  void FrameBufferResizeCallback(int w, int h);
  void MouseCallback(double xpos, double ypos);
  void ScrollCallback(double xpos, double yPos);
  void HandleInput(int key, int scancode, int action, int mod);
  void addResizeCallback(std::function<void(int, int)> callback) {
    registeredResizeCallbacks.push_back(callback);
  }
  bool isOpen();
};
#endif
