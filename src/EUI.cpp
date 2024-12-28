#include "../include/EUI.h"
#include <algorithm>
#include <memory>
std::vector<ObjectLoaderEntry> entries{
    {"Cube", AddCube},
    {"Plane", AddPlane},
    {"Sphere", AddSphere},
    {"Torus", AddTorus},

};
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
  if (ImGui::Begin("Hierarchy", nullptr,
                   ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar)) {

    if (ImGui::BeginMenuBar()) {
      if (ImGui::BeginMenu("Add")) {

        for (auto &entry : entries) {
          if (ImGui::MenuItem(entry.entry_name.c_str())) {
            entry.action(state);
          }
        }
        if (ImGui::BeginMenu("Light")) {

          if (ImGui::MenuItem("Ambient")) {
            state.World->AddLight(
                createRef<AmbientLight>(glm::vec3(1.0f), 0.6f));
            state.World->setRecompiled(false);
          }

          if (ImGui::MenuItem("Directioanl")) {
            state.World->AddLight(createRef<DirectionalLight>(
                glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3(0.0f, 0.0f, 1.0f)));
            state.World->setRecompiled(false);
          }

          if (ImGui::MenuItem("Spot")) {
            state.World->AddLight(createRef<SpotLight>(
                glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
            state.World->setRecompiled(false);
          }
          if (ImGui::MenuItem("Point")) {
            state.World->AddLight(createRef<PointLight>(
                glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3(0.0f, 0.0f, 0.0f)));
            state.World->setRecompiled(false);
          }

          ImGui::EndMenu();
        }
        ImGui::EndMenu();
      }
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
      if (ImGui::Button("Remove Object")) {

        if (state.curSelectedObject != state.World) {
          state.World->remove(state.curSelectedObject);

          // TODO: this could be improved

          state.curSelectedObject = nullptr;
        }
      }
      if (ImGui::Button("Remove Light")) {

        if (state.curSelectedLight) {
          state.World->Lights.erase(std::remove(state.World->Lights.begin(),
                                                state.World->Lights.end(),
                                                state.curSelectedLight),
                                    state.World->Lights.end());
          state.curSelectedLight = nullptr;

          state.World->setRecompiled(false);
        }
      }

      ImGui::PopStyleColor();

      ImGui::EndMenuBar();
    }

    ImGui::SeparatorText("Objects");

    TraverseObjects(state, std::static_pointer_cast<EObject3D>(state.World));

    ImGui::SeparatorText("Lights");
    TraverseLights(state);
    ImGui::End();
  }
  ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 300, 0),
                          ImGuiCond_Once);
  if (ImGui::Begin("Options")) {
    if (state.curSelectedObject && state.curSelectedObject != state.World) {

      if (state.curSelectedObject && state.curSelectedObject != state.World) {

        std::string obj_name = state.curSelectedObject->getName();
        if (obj_name.length() >= 20)
          obj_name = obj_name.substr(0, 15) + "...";
        if (ImGui::CollapsingHeader((obj_name + " Properties").c_str())) {
          ImGui::SeparatorText(state.curSelectedObject->getName().c_str());

          // Position Controls (translation)
          ImGui::Text("Position");
          glm::vec3 currentPos = state.curSelectedObject->getPosition();
          if (ImGui::SliderFloat3("Position", &currentPos[0], -100.0f,
                                  100.0f)) {
            // Update the position of the selected object
            state.curSelectedObject->setPosition(currentPos.x, currentPos.y,
                                                 currentPos.z);
          }

          // Rotation Controls (Euler angles - pitch, yaw, roll)
          ImGui::Text("Rotation");
          glm::vec3 currentRotation = state.curSelectedObject->getRotation();
          if (ImGui::SliderFloat3("Rotation (Degrees)", &currentRotation[0],
                                  -180.0f, 180.0f)) {
            // Update the rotation (convert degrees to radians if necessary)
            state.curSelectedObject->setRotation(
                currentRotation.x, currentRotation.y, currentRotation.z);
          }

          // Scale Controls
          ImGui::Text("Scale");
          glm::vec3 currentScale = state.curSelectedObject->getScale();
          if (ImGui::SliderFloat3("Scale", &currentScale[0], 0.1f, 10.0f)) {
            // Update the scale of the selected object
            state.curSelectedObject->setScale(currentScale.x, currentScale.y,
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
    if (state.curSelectedLight) {
      std::string light_name = state.curSelectedLight->getName();
      if (light_name.length() >= 20)
        light_name = light_name.substr(0, 15) + "...";

      if (ImGui::CollapsingHeader(light_name.c_str())) {

        // Light properties

        // Position and Translation
        ImGui::Text("Position");
        glm::vec3 cur_pos = state.curSelectedLight->getPosition();

        if (ImGui::SliderFloat3("Light Position", &cur_pos[0], -100.0f,
                                100.0f)) {

          state.curSelectedLight->setPosition(cur_pos.x, cur_pos.y, cur_pos.z);
        }

        if (state.curSelectedLight->type == LightType::DIRECTIONAL ||
            state.curSelectedLight->type == LightType::SPOTLIGHT) {

          static glm::vec3 dir =
              state.curSelectedLight->direction; // Keep a local editable copy

          // Show sliders for direction components
          if (ImGui::SliderFloat3("Light Direction", &dir[0], -1.0f, 1.0f)) {
            // Normalize the vector after editing to ensure it's a valid
            // direction
            state.curSelectedLight->direction = glm::normalize(dir);
          }
        }
        ImGui::Text("Intensity");
        if (ImGui::SliderFloat("Light Intensity",
                               &state.curSelectedLight->intensity, 0.0f,
                               100.0f)) {
        }

        ImGui::Text("Color");
        glm::vec3 color;

        // Ambient Color
        color = state.curSelectedLight->ambient_color;
        if (ImGui::ColorEdit3("Ambient Color", &color[0])) {
          state.curSelectedLight->ambient_color = color;
        }

        // Diffuse Color
        color = (state.curSelectedLight->diffuse_color);
        if (ImGui::ColorEdit3("Diffuse Color", &color[0])) {
          state.curSelectedLight->diffuse_color = color;
        }

        // Specular Color
        color = state.curSelectedLight->specular_color;
        if (ImGui::ColorEdit3("Specular Color", &color[0])) {
          state.curSelectedLight->specular_color = color;
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

      ImGui::Checkbox("Grid", &state.rendererState.enableGrid);
      ImGui::Checkbox("Depth Testing", &state.rendererState.enableDepthTesting);
      ImGui::Checkbox("Stencil Testing",
                      &state.rendererState.enableStencilTesting);
      ImGui::Checkbox("Backface Culling", &backfaceCulling);
      ImGui::Checkbox("Blending", &blending);
      bool pst = state.rendererState.enableNormalMapping;
      ImGui::Checkbox("Normal Mapping",
                      &state.rendererState.enableNormalMapping);
      if (pst != state.rendererState.enableNormalMapping) {
        state.rendererState.update = true;
      }

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

  if (state.rendererState.enableGrid) {
    renderer.EnableGrid();
  } else
    renderer.DisableGrid();

  if (state.rendererState.update) {
    renderer.setNormalMapping(state.rendererState.enableNormalMapping);
  }
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

void TraverseLights(EngineState &state) {
  if (state.World) {
    for (auto &light : state.World->Lights) {

      if (ImGui::Selectable(light->getName().c_str(),
                            state.curSelectedLight == light)) {
        state.curSelectedLight = light;
      }
    }
  }
}
void TraverseObjects(EngineState &state, std::shared_ptr<EObject3D> obj) {
  if (!obj)
    return;

  ImGui::PushID(obj.get());
  if (obj->hasChildren()) {
    // has children, change the rendering logic to

    ImGuiTreeNodeFlags flag = ImGuiTreeNodeFlags_OpenOnArrow;

    if (state.curSelectedObject == obj) {
      flag |= ImGuiTreeNodeFlags_Selected;
    }
    if (ImGui::TreeNodeEx(obj->getName().c_str(), flag)) {

      if (ImGui::IsItemClicked()) {
        state.curSelectedObject = obj;
        std::cout << "Selected " << obj->getName() << std::endl;
      }
      for (auto &d : obj->getChildren()) {
        TraverseObjects(state, d);
      }
      ImGui::TreePop();
    }

  } else {

    if (ImGui::Selectable(obj->getName().c_str(),
                          state.curSelectedObject == obj)) {
      state.curSelectedObject = obj;
      std::cout << "Selected " << obj->getName() << std::endl;
    }
  }
  ImGui::PopID();
}

void AddCube(EngineState &state) {
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
    if (state.curSelectedObject) {
      state.curSelectedObject->add(cube);
    } else {
      state.World->add(cube);
    }

    state.World->setRecompiled(false);
  }

  return;
}
void AddSphere(EngineState &state) {
  if (state.World) {
    UniformMap uniforms = {{"time", 0.0f}, {"uAmp", 2.0f}};

    auto tex = ETexture::load(
        "../models/textures/3DLABbg_UV_Map_Checker_01_2048x2048.jpg");
    uniforms["tex"] = tex;
    auto mat = std::make_shared<ShaderMaterial>(
        "../shaders/vertex/basic.glsl", "../shaders/fragment/basic.glsl",
        uniforms);

    auto sphere =
        createRef<EMesh>(createRef<EUVSphereGeometry>(1.0f, 30, 30), mat);
    if (state.curSelectedObject) {
      state.curSelectedObject->add(sphere);
    } else {
      state.World->add(sphere);
    }

    state.World->setRecompiled(false);
  }
}
void AddPlane(EngineState &state) {

  if (state.World) {
    UniformMap uniforms = {{"time", 0.0f}, {"uAmp", 2.0f}};

    auto tex = ETexture::load(
        "../models/textures/3DLABbg_UV_Map_Checker_01_2048x2048.jpg");
    uniforms["tex"] = tex;
    auto mat = std::make_shared<ShaderMaterial>(
        "../shaders/vertex/basic.glsl", "../shaders/fragment/basic.glsl",
        uniforms);

    auto plane = createRef<EMesh>(createRef<EPlaneGeometry>(), mat);
    if (state.curSelectedObject) {
      state.curSelectedObject->add(plane);
    } else {
      state.World->add(plane);
    }

    state.World->setRecompiled(false);
  }
}
void AddTorus(EngineState &state) { return; }
