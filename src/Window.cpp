#include "../include/Window.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
#include <stdio.h>

Window::Window(int w, int h, std::string title, bool fullscreen) {

  size_.w = w;
  size_.h = h;

  // centering cursor position
  cursorPosX = (float)size_.w / 2.0f;
  cursorPosY = (float)(size_.h) / 2.0f;

  fullscreen_ = fullscreen;

  if (glfwInit() != GLFW_TRUE) {
    // TODO:  something about it
  }

  // hinting glfw
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window_ = glfwCreateWindow(w, h, title.c_str(), nullptr, nullptr);

  if (!window_) {

    // TODO: create an exception class
    printf("Couldn't Create window_\n");
  }

  // creating imgui context
  glfwSetWindowUserPointer(window_, this);

  // activate and set viewport
  Activate();

  if (!(gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress)))) {

    printf("Couldn't Init Glad_\n");
  }
  isOpen_ = true;
  SetViewport();
  // start enabling stuff
  //

  EnableKeyInput();
  EnableResizing();
  EnableVsync();
  DisableCursor();
  EnableScrollInput();
  EnableMouseInput();

  // create imgui context
  ImGui::CreateContext();
  imguiIO = &ImGui::GetIO();
  imguiIO->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  ImGui_ImplGlfw_InitForOpenGL(window_, true);
  ImGui_ImplOpenGL3_Init("#version 330");
};
Window::~Window() { DestroyWindow(); }

void Window::Activate() { glfwMakeContextCurrent(window_); }

void Window::SetViewport() { glViewport(0, 0, size_.w, size_.h); }

void Window::EnableResizing() {

  if (resizeUpdatesEnabled_)
    return;

  auto callback = [](GLFWwindow *window, int w, int h) {
    auto self = (Window *)glfwGetWindowUserPointer(window);
    if (self != nullptr) {
      self->FrameBufferResizeCallback(w, h);
    }
  };

  glfwSetFramebufferSizeCallback(window_, (GLFWframebuffersizefun)callback);
  resizeUpdatesEnabled_ = true;
}

void Window::DisableCursor() {
  if (!cursorEnabled_)
    return;
  glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  cursorEnabled_ = false;
}
void Window::EnableCursor() {
  if (cursorEnabled_)
    return;
  glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

// TODO: decouple window and camera
// in the current set up, multiple cameras
// are a distant dream
void Window::BindCamera(std::shared_ptr<Camera> camera) {
  camera_ = camera;
  cameraBound = true;
  camera_->SetAspectRatio(GetAspectRatio());
}
void Window::EnableMouseInput() {
  if (mouseInputEnabled_)
    return;

  auto callback = [](GLFWwindow *window, double xpos, double ypos) {
    auto self = (Window *)(glfwGetWindowUserPointer(window));

    if (self != nullptr) {
      self->MouseCallback(xpos, ypos);
    }
  };
  glfwSetCursorPosCallback(window_, callback);
  mouseInputEnabled_ = true;
}
void Window::DisableMouseInput() {
  if (!mouseInputEnabled_)
    return;

  glfwSetCursorPosCallback(window_, nullptr);
}

void Window::EnableScrollInput() {
  if (scollInputEnabled_)
    return;

  auto callback = [](GLFWwindow *window, double xpos, double ypos) {
    auto self = (Window *)glfwGetWindowUserPointer(window);
    if (self != nullptr) {
      self->ScrollCallback(xpos, ypos);
    }
  };
  glfwSetScrollCallback(window_, callback);
  scollInputEnabled_ = true;
}
void Window::MouseCallback(double xpos, double ypos) {

  if (imguiIO->WantCaptureMouse)
    return;

  float deltaX = cursorPosX - xpos;
  float deltaY = cursorPosY - ypos;
  cursorPosX = xpos;
  cursorPosY = ypos;
  // durring start up there is a very sudden movement of mouse cursor, this will
  // create a huge delta X and delta Y values
  if (firstMove) {
    deltaX = 0.0f;
    deltaY = 0.0f;
    firstMove = false;
  } else {
    if (glfwGetMouseButton(window_, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS)
      return;
    if (cameraBound) {
      camera_->ProcessMouseMovement(deltaX, deltaY, true);
    }
  }
}
void Window::ScrollCallback(double xoffset, double yoffset) {
  (void)(xoffset);
  if (imguiIO->WantCaptureMouse)
    return;
  if (cameraBound) {
    camera_->ProcessScroll(yoffset);
  }
}
void Window::FrameBufferResizeCallback(int w, int h) {

  glViewport(0, 0, w, h);
  size_.w = w;
  size_.h = h;
  if (cameraBound) {
    camera_->SetAspectRatio((float)w / (float)h);
  }
  for (const auto &cbk : registeredResizeCallbacks) {
    cbk(w, h);
  }
}
void Window::EnableKeyInput() {
  if (keyInputEnabled_)
    return;

  auto callback = [](GLFWwindow *window, int key, int scancode, int action,
                     int mod) {
    auto self = (Window *)(glfwGetWindowUserPointer(window));
    if (self != nullptr) {
      self->HandleInput(key, scancode, action, mod);
    }
  };
  glfwSetKeyCallback(window_, callback);
  keyInputEnabled_ = true;
}

void Window::HandleInput(int key, int scancode, int action, int mod) {

  if (imguiIO->WantCaptureKeyboard || imguiIO->WantTextInput)
    return;
  // TODO: This is bad
  if (key == GLFW_KEY_ESCAPE) {
    glfwWindowShouldClose(window_);
    DestroyWindow();
  }
  if (cameraBound) {
    CameraMovement movement = CameraMovement::STATIC;
    if (action == GLFW_PRESS || GL_REPEAT) {
      switch (key) {
      case GLFW_KEY_W: {
        movement = CameraMovement::FORWARD;
        break;
      }
      case GLFW_KEY_S: {
        movement = CameraMovement::BACKWARD;
        break;
      }
      case GLFW_KEY_A: {
        movement = CameraMovement::LEFT;
        break;
      }
      case GLFW_KEY_D: {
        movement = CameraMovement::RIGHT;
      }
      }
    }

    camera_->ProcessInput(movement, deltaTime);
  }
}

bool Window::isOpen() { return (window_ != nullptr && isOpen_); }
void Window::DestroyWindow() {
  // closing imgui
  //
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();

  ImGui::DestroyContext();

  glfwSetWindowUserPointer(window_, nullptr);
  glfwSetWindowShouldClose(window_, true);
  glfwDestroyWindow(window_);
  isOpen_ = false;
  window_ = nullptr;
  glfwTerminate();
}
void Window::EnableVsync() {
  if (vsyncEnabled)
    return;
  glfwSwapInterval(1);
  vsyncEnabled = true;
}
void Window::DisableVsync() {
  if (!vsyncEnabled)
    return;
  glfwSwapInterval(0);
  vsyncEnabled = false;
}
void Window::Update() {
  float curTime = glfwGetTime();
  deltaTime = curTime - lastFrameTime_;
  lastFrameTime_ = curTime;
  glfwPollEvents();
  glfwSwapBuffers(window_);
}
void Window::UpdateUniforms(Shader &shader) {

  // change in time between the last frame and current frame
  shader.setFloat("uDeltaTime", deltaTime);
  // total time elapsed since the start of the program
  shader.setFloat("uTime", glfwGetTime());
  shader.setVec2("uResolution", glm::vec2(size_.w, size_.h));

  // resolution of screen
  if (cameraBound) {
    camera_->SetAspectRatio(GetAspectRatio());
    camera_->UpdateUniforms(shader);
  }
}
float Window::GetAspectRatio() { return (float)size_.w / (float(size_.h)); }
GLFWwindow *Window::GetRef() { return window_; }
