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

    if (ImGui::Button("Add Object")) {

      // for test let's just create a new cube lmao
      if (state.World) {
        UniformMap uniforms = {{"time", 0.0f}, {"uAmp", 2.0f}};

        auto tex = ETexture::load(
            "../models/textures/3DLABbg_UV_Map_Checker_01_2048x2048.jpg");
        uniforms["tex"] = tex;
        auto mat = std::make_shared<ShaderMaterial>(
            "../shaders/vertex/basic.glsl", "../shaders/fragment/basic.glsl",
            uniforms);

        auto cube = createRef<EMesh>(createRef<EBoxGeometry>(),
                                     mat); // `cube` is a shared_ptr already
        if (state.curSelected) {
          printf("Adding to curSelected- %s : \n",
                 state.curSelected->getName().c_str());

          std::cout << "Before adding, cube use count: " << cube.use_count()
                    << std::endl;
          state.curSelected->add(cube);
          std::cout << "After adding, cube use count: " << cube.use_count()
                    << std::endl;
        } else {
          printf("Adding to world: \n");
          std::cout << "Before adding, cube use count: " << cube.use_count()
                    << std::endl;
          state.World->add(cube);
          std::cout << "After adding, cube use count: " << cube.use_count()
                    << std::endl;
        }

        std::cout << "Added to " << state.World->getName() << " !\n";
        state.World->setRecompiled(false);
      }
    }
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
    if (ImGui::Button("Remove Object")) {

      if (state.curSelected != state.World) {
        state.World->remove(state.curSelected);

        // TODO: this could be improved

        state.curSelected = nullptr;
      }
    }
    ImGui::PopStyleColor();

    ImGui::SeparatorText("Objects");

    traverse(state, std::static_pointer_cast<EObject3D>(state.World));

    ImGui::End();
  }
  ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 300, 0),
                          ImGuiCond_Once);
  if (ImGui::Begin("Options")) {
    if (state.curSelected && state.curSelected != state.World) {

      if (state.curSelected && state.curSelected != state.World) {
        if (ImGui::CollapsingHeader("Object")) {
          ImGui::SeparatorText(state.curSelected->getName().c_str());

          // Position Controls (translation)
          ImGui::Text("Position");
          glm::vec3 currentPos = state.curSelected->getPosition();
          if (ImGui::SliderFloat3("Position", &currentPos[0], -100.0f,
                                  100.0f)) {
            // Update the position of the selected object
            state.curSelected->setPosition(currentPos.x, currentPos.y,
                                           currentPos.z);
          }

          // Rotation Controls (Euler angles - pitch, yaw, roll)
          ImGui::Text("Rotation");
          glm::vec3 currentRotation = state.curSelected->getRotation();
          if (ImGui::SliderFloat3("Rotation (Degrees)", &currentRotation[0],
                                  -180.0f, 180.0f)) {
            // Update the rotation (convert degrees to radians if necessary)
            state.curSelected->setRotation(currentRotation.x, currentRotation.y,
                                           currentRotation.z);
          }

          // Scale Controls
          ImGui::Text("Scale");
          glm::vec3 currentScale = state.curSelected->getScale();
          if (ImGui::SliderFloat3("Scale", &currentScale[0], 0.1f, 10.0f)) {
            // Update the scale of the selected object
            state.curSelected->setScale(currentScale.x, currentScale.y,
                                        currentScale.z);
          }

          // Optionally, display the current position, rotation, and scale
          ImGui::Text("Current Position: %.2f, %.2f, %.2f", currentPos.x,
                      currentPos.y, currentPos.z);
          ImGui::Text("Current Rotation: %.2f, %.2f, %.2f", currentRotation.x,
                      currentRotation.y, currentRotation.z);
          ImGui::Text("Current Scale: %.2f, %.2f, %.2f", currentScale.x,
                      currentScale.y, currentScale.z);
        }
      }
    }
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
      static bool backfaceCulling = false;
      static bool blending = true;

      ImGui::Checkbox("Depth Testing", &state.rendererState.enableDepthTesting);
      ImGui::Checkbox("Stencil Testing",
                      &state.rendererState.enableStencilTesting);
      ImGui::Checkbox("Backface Culling", &backfaceCulling);
      ImGui::Checkbox("Blending", &blending);

      ImGui::SeparatorText("Poly Fill Mode");

      int current_idx = (int)(state.rendererState.polymode);

      const char *modes[] = {"Fill", "Line", "Point"};

      if (ImGui::BeginCombo("PolyMode", modes[current_idx])) {

        for (int i = 0; i < IM_ARRAYSIZE(modes); i++) {
          bool isSelected = (current_idx == i);

          if (ImGui::Selectable(modes[i], isSelected)) {
            current_idx = i;
          }
        }
        ImGui::EndCombo();
      }
      state.rendererState.polymode = (PolyMode)(current_idx);

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

      ImGui::Checkbox("Enable Skybox", &state.worldstate.skyboxEnabled);

      auto items = state.worldstate.cubeMaps;

      if (!state.worldstate.skyboxEnabled) {
        ImGui::BeginDisabled();
      }

      std::string label = "";
      int idx = state.worldstate.curSkyboxIdx;

      // Check if idx is valid before accessing cubeMaps
      if (idx >= 0 && idx < (int)state.worldstate.cubeMaps.size()) {
        label = state.worldstate.cubeMaps[idx];
      }

      if (ImGui::BeginCombo("Skyboxes", label.c_str())) {
        for (int i = 0; i < (int)state.worldstate.cubeMaps.size(); i++) {
          bool isSelected = idx == i;
          if (ImGui::Selectable(state.worldstate.cubeMaps[i].c_str(),
                                isSelected)) {
            if (!isSelected) {
              state.worldstate.curSkyboxIdx = i;
              state.worldstate.skyboxReloaded = true;
            }
          }

          // Ensure only one item can remain selected
          if (isSelected) {
            ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo(); // Make sure to end the combo
      }

      if (!state.worldstate.skyboxEnabled) {
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
void UpdateRenderer(ERenderer &renderer, EngineState &state) {

  if (state.rendererState.enableDepthTesting) {
    renderer.EnableDepthTesting();
  } else
    renderer.DisableDepthTesting();

  if (state.rendererState.enableStencilTesting) {
    renderer.EnableStencilTesting();
  } else
    renderer.DisableStencilTesting();

  renderer.setPolyMode(state.rendererState.polymode);
}
void UpdateWorld(EWorld &world, EngineState &state) {
  if (!state.worldstate.skyboxEnabled) {
    world.unloadCubeMaps();
    return;
  }
  if (state.worldstate.skyboxReloaded) {

    // load a new cubemap
    if (state.worldstate.curSkyboxIdx >= 0)
      world.unloadCubeMaps();
    world.loadCubeMaps(
        state.worldstate.cubeMaps[state.worldstate.curSkyboxIdx]);
  }

  state.worldstate.skyboxReloaded = false;
}
void traverse(EngineState &state, std::shared_ptr<EObject3D> obj) {
  if (!obj)
    return;

  ImGui::PushID(obj.get());
  if (obj->hasChildren()) {
    // has children, change the rendering logic to

    ImGuiTreeNodeFlags flag = ImGuiTreeNodeFlags_OpenOnArrow;

    if (state.curSelected == obj) {
      flag |= ImGuiTreeNodeFlags_Selected;
    }
    if (ImGui::TreeNodeEx(obj->getName().c_str(), flag)) {

      if (ImGui::IsItemClicked()) {
        state.curSelected = obj;
        std::cout << "Selected " << obj->getName() << std::endl;
      }
      for (auto &d : obj->getChildren()) {
        traverse(state, d);
      }
      ImGui::TreePop();
    }

  } else {

    if (ImGui::Selectable(obj->getName().c_str(), state.curSelected == obj)) {
      state.curSelected = obj;
      std::cout << "Selected " << obj->getName() << std::endl;
    }
  }
  ImGui::PopID();
}
