#include "../include/EUI.h"
void UpdateEngine(Window &window, EngineState &state) {

  if (state.vsyncEnabled) {
    window.EnableVsync();
  } else
    window.DisableVsync();
}

void StartUIFrame() {

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}
void RenderUI(EngineState &state) {
  ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
  if (ImGui::Begin("Hierarchy"), ImGuiWindowFlags_NoCollapse) {
    ImGui::SeparatorText("World");

    if (ImGui::Button("Add Object")) {
    }
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
    if (ImGui::Button("Remove Object")) {
    }
    ImGui::PopStyleColor();

    static bool check = false;
    ImGui::Checkbox("Checkbox", &check);
    ImGui::End();
  }

  if (ImGui::Begin("Options")) {

    if (ImGui::CollapsingHeader("Application")) {
      ImGui::Indent(8);
      ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
      ImGui::Text("Frame Time: %.3f ms", 1000.0f / ImGui::GetIO().Framerate);
      ImGui::Checkbox("VSync", &state.vsyncEnabled);
      ImGui::Unindent(8);
    }

    if (ImGui::CollapsingHeader("Renderer")) {
      ImGui::Indent(8);
      ImGui::SeparatorText("Renderer Options");
      static bool depthTesting = true;
      static bool stencilTesting = false;
      static bool backfaceCulling = false;
      static bool blending = true;

      ImGui::Checkbox("Depth Testing", &depthTesting);
      ImGui::Checkbox("Stencil Testing", &stencilTesting);
      ImGui::Checkbox("Backface Culling", &backfaceCulling);
      ImGui::Checkbox("Blending", &blending);

      ImGui::Unindent(8);
    }

    if (ImGui::CollapsingHeader("Camera")) {
      ImGui::Indent(8);

      ImGui::SeparatorText("ZFar/ZNear");
      ImGui::SliderFloat("zNear", &state.cameraState.clipPlanes.x, 0.1, 100.0f);
      ImGui::SliderFloat("zFar", &state.cameraState.clipPlanes.y, 100.0f,
                         1000.0f);

      ImGui::SeparatorText("Position");

      if (ImGui::DragFloat("xPos", &state.cameraState.Poition.x)) {
        state.cameraState.updateByUI = true;
      }

      if (ImGui::DragFloat("yPos", &state.cameraState.Poition.y)) {
        state.cameraState.updateByUI = true;
      }

      if (ImGui::DragFloat("zPos", &state.cameraState.Poition.z)) {
        state.cameraState.updateByUI = true;
      }

      ImGui::SeparatorText("Orientation");

      if (ImGui::SliderFloat("Yaw", &state.cameraState.Orientation.x, -360.0f,
                             360.0f)) {
        state.cameraState.updateByUI = true;
      }
      if (ImGui::SliderFloat("Pitch", &state.cameraState.Orientation.y, -360.0f,
                             360.0f)) {
        state.cameraState.updateByUI = true;
      }
      if (ImGui::SliderFloat("Roll", &state.cameraState.Orientation.z, -360.0f,
                             360.0f)) {
        state.cameraState.updateByUI = true;
      }

      ImGui::Unindent(8);
    }
    if (ImGui::CollapsingHeader("Scene")) {
      ImGui::Indent(8);

      static bool skyboxEnable = true;

      ImGui::Checkbox("Enable Skybox", &skyboxEnable);
      static int selected_item = 1;

      const char *items[] = {"Blue Sky", "Nebula", "MilkyWay"};

      if (!skyboxEnable) {
        ImGui::BeginDisabled();
      }
      if (ImGui::Combo("Skybox", &selected_item, items, IM_ARRAYSIZE(items))) {
      }
      if (!skyboxEnable) {
        ImGui::EndDisabled();
      }
      ImGui::Unindent(8);
    }

    ImGui::End();
  }
}

void EndUIFrame() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UpdateCamera(Camera &camera, EngineState &state) {

  if (state.cameraState.updateByUI) {
    camera.setPosition(state.cameraState.Poition.x, state.cameraState.Poition.y,
                       state.cameraState.Poition.z);
    camera.setOrientation(state.cameraState.Orientation);
  } else {
    // synchronize
    state.cameraState.Poition = camera.getPosition();
    state.cameraState.Orientation = camera.getOrientation();
  }
  camera.setNearFarPlanes(state.cameraState.clipPlanes.x,
                          state.cameraState.clipPlanes.y);
  camera.setFov(state.cameraState.Fov);
  state.cameraState.updateByUI = false;
}
