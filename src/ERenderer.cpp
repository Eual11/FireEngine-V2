#include "../include/ERenderer.h"
#include "../include/EPostProcessingPipeline.h"
#include "../include/EQuadGeometry.h"
#include <memory>

ERenderer::ERenderer() {
  EnableDepthTesting();
  setDepthTestFunc(GL_LESS);
}

ERenderer::ERenderer(Window *window) {
  this->window = window;

  EnableDepthTesting();
  EnableStencilTesting();
  setDepthTestFunc(GL_LESS);

  outlineShader =
      std::make_shared<Shader>("../shaders/vertex/basic.glsl",
                               "../shaders/fragment/normal_material.glsl");

  auto geo = std::make_shared<EQuadGeometry>(1.0f, 1.0f);

  auto mat = std::make_shared<ShaderMaterial>("../shaders/vertex/grid.glsl",
                                              "../shaders/fragment/grid.glsl");

  fGrid = std::make_shared<EMesh>(geo, mat);
  auto qd = std::static_pointer_cast<EObject3D>(fGrid);
  CompileMeshShader(qd);
  effectPipeline = EPostProcessingPipeline(window);
  effectPipeline.Init();
}

void ERenderer::EnableDepthTesting() {
  if (!depthTestingEnabled) {
    glEnable(GL_DEPTH_TEST);
    depthTestingEnabled = true;
    clearBit |= GL_DEPTH_BUFFER_BIT;
  }
}
void ERenderer::EnableStencilTesting() {
  if (!stencilTestingEnabled) {
    glEnable(GL_STENCIL_TEST);
    stencilTestingEnabled = true;
    clearBit |= GL_STENCIL_BUFFER_BIT;
  }
}
void ERenderer::DisableStencilTesting() {
  if (stencilTestingEnabled) {
    glDisable(GL_STENCIL_TEST);
    stencilTestingEnabled = false;
    clearBit &= (~GL_STENCIL_BUFFER_BIT);
  }
}
void ERenderer::DisableDepthTesting() {
  if (depthTestingEnabled) {
    glDisable(GL_DEPTH_TEST);
    depthTestingEnabled = false;
    clearBit &= (~GL_DEPTH_BUFFER_BIT);
  }
}
void inline ERenderer::setClearColor(float r, float g, float b, float a) {
  clearColor = {r, g, b, a};
  glClearColor(r, g, b, a);
}
void ERenderer::setDepthTestFunc(unsigned int func) { glDepthFunc(func); }

void ERenderer::Render(std::shared_ptr<EWorld> &world) {
  // binding the input buffer for the effects pipeline, i.e the input is the
  // renderered world
  // pipe the output of framebuffer to the window's framebuffer with
  // post-processing (currently only forward rendering post post-processing)
  //
  effectPipeline.getInputFramebuffer()->Bind();
  glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
  glClear(clearBit);
  if (world->hasSkyBox) {
    glDepthFunc(GL_LEQUAL);
    RenderSkybox(world);
    glDepthFunc(GL_LESS);
  }
  if (!world->shouldRecompileMaterials()) {
    // compiles shaders assosiated with the game world, currently compiles
    // shaders for the skybox
    /* CompileWorldShader(world); */
    try {
      CompileShaders(world);
    } catch (std::bad_weak_ptr) {
      printf("Bad A weak ptr\n");
      return;
    }

    // materials have been recompiled
    world->setRecompiled(true);
  }

  for (auto &child : world->getChildren()) {

    if (shader_map.find(child) != shader_map.end()) {
      Shader &shader = *shader_map[child].get();

      if (window) {
        // rendering to default framebuffer
        window->UpdateUniforms(shader);
        shader.setBool("enableNormalMapping", normalMapping);
        window->UpdateUniforms(*outlineShader);
        CalculateLighting(world, shader);

        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);

        child->render(shader);

        if (outlinesEnabled) {
          auto scale = child->getScale();

          child->setScale(scale.x * outlineSize, scale.y * outlineSize,
                          scale.z * outlineSize);

          // enable writing to stencil buffer
          DisableDepthTesting();
          glStencilMask(0x00);
          glStencilFunc(GL_NOTEQUAL, 1, 0xFF);

          child->render(*outlineShader);
          // scaling back to original
          child->setScale(scale.x, scale.y, scale.z);
          EnableDepthTesting();
          glStencilMask(0xFF);
          glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        }
      }
    } else
      FetchShaderAndRender(world, child);
  }

  if (grid) {
    if (shader_map.find(fGrid) != shader_map.end()) {
      Shader &shader = *shader_map[fGrid].get();
      window->UpdateUniforms(shader);
      fGrid->render(shader);
    }
  }

  if (window) {
    // effects should be able to depth value to the depth buffer
    glDepthMask(GL_FALSE);
    DisableDepthTesting();
    effectPipeline.applyEffects();
    // enabling it back, it is etiquette to revert what you did
    glDepthMask(GL_TRUE);
    EnableDepthTesting();
  }
}

void ERenderer::RenderSkybox(std::shared_ptr<EWorld> &world) {
  if (shader_map.find(world) != shader_map.end()) {
    glDepthMask(GL_FALSE);
    glStencilMask(0x00);
    Shader &skyboxShader = *shader_map[world];
    skyboxShader.Use();
    Window *attachedWindow = window;
    if (attachedWindow) {
      attachedWindow->UpdateUniforms(skyboxShader);
      if (attachedWindow->getBoundCamera()) {
        glm::mat4 view =
            glm::mat4(glm::mat3(attachedWindow->getBoundCamera()->getLookAt()));
        skyboxShader.setMat4("uView", view);
      }
    }
    glBindVertexArray(world->getSkyboxVAO());
    glBindTexture(GL_TEXTURE_CUBE_MAP, world->getSkyboxCubeMap());
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);
    glStencilMask(0xFF);
  }
}
void ERenderer::CompileShaders(std::shared_ptr<EObject3D> object) {

  if (!object)
    return;

  if (object->getType() == EObject3D::Type::Mesh) {
    CompileMeshShader(object);
  } // if it's a mesh set up materials and everything
  else if (object->getType() == EObject3D::Type::Model) {
    CompileModelShader(object);
  } else if (object->getType() == EObject3D::Type::World) {
    auto world = std::dynamic_pointer_cast<EWorld>(object);
    CompileWorldShader(world);
  }

  for (auto &child : object->getChildren()) {

    CompileShaders(child); // then recursively go through the childs
  }
}
void ERenderer::CompileMeshShader(std::shared_ptr<EObject3D> &object) {
  auto mesh = std::dynamic_pointer_cast<EMesh>(object);

  if (mesh && mesh->getMaterial()) {
    // HACK: this must be replaced
    std::string vertSrc = default_vertex_shader_source;
    std::string fragSrc = "";

    if (default_fragment_shader_sources.find(mesh->getMaterial()->type) !=
        default_fragment_shader_sources.end()) {
      fragSrc = default_fragment_shader_sources[mesh->getMaterial()->type];

      /* std::cout << "fragSrc: " << fragSrc << std::endl; */
      std::string shader_src = vertSrc + fragSrc;
      if (compiled_shaders.find(shader_src) == compiled_shaders.end()) {

        auto cur_shader =
            std::make_shared<Shader>(vertSrc.c_str(), fragSrc.c_str());
        compiled_shaders[shader_src] = cur_shader;
        shader_map[mesh] = cur_shader;
      } else {

        shader_map[mesh] = compiled_shaders[shader_src];
      }
    } else if (mesh->getMaterial()->type == MaterialType_Shader) {

      auto material =
          std::dynamic_pointer_cast<ShaderMaterial>(mesh->getMaterial());

      if (material) {
        std::string shader_src =
            material->vertexShaderSrc + material->fragmentShaderSrc;
        if (compiled_shaders.find(shader_src) == compiled_shaders.end()) {
          auto cur_shader =
              std::make_shared<Shader>(material->vertexShaderSrc.c_str(),
                                       material->fragmentShaderSrc.c_str());
          compiled_shaders[shader_src] = cur_shader;
          shader_map[mesh] = cur_shader;
        } else {

          shader_map[mesh] = compiled_shaders[shader_src];
        }
      }
    }
  }
  // recursively compile mesh shaders for children
}
void ERenderer::CompileWorldShader(std::shared_ptr<EWorld> &world) {

  if (world) {
    // HACK: this must be replaced
    std::string vertSrc = default_skybox_shader_source;
    std::string fragSrc = "../shaders/fragment/skybox.glsl";
    std::string shader_src = vertSrc + fragSrc;
    if (compiled_shaders.find(shader_src) == compiled_shaders.end()) {
      auto cur_shader =
          std::make_shared<Shader>(vertSrc.c_str(), fragSrc.c_str());
      compiled_shaders[shader_src] = cur_shader;
      shader_map[world] = cur_shader;
    }
  }
}
void ERenderer::CalculateLighting(const std::shared_ptr<EWorld> &world,
                                  Shader &shader) {

  unsigned int dirCount = 0;
  unsigned int pointCount = 0;
  unsigned int spotCount = 0;

  // NOTE: this piece of code below is awful and disgusting
  for (auto &light : world->Lights) {
    switch (light->type) {
    case AMBIENT: {
      auto Alight = std::dynamic_pointer_cast<AmbientLight>(light);
      if (Alight) {
        shader.setVec3("ambientLight", Alight->ambient_color);
        shader.setFloat("ambientIntensity", Alight->intensity);
      }

      break;
    }
    case DIRECTIONAL: {

      if (dirCount < MAX_LIGHT_COUNT) {
        auto Dlight = std::dynamic_pointer_cast<DirectionalLight>(light);

        if (Dlight) {
          std::string light_ref = "dirLights";
          light_ref += "[" + std::to_string(dirCount) + "]";
          shader.setBool(light_ref + ".light_active", true);
          shader.setVec3(light_ref + ".diffuse_color", Dlight->diffuse_color);
          shader.setVec3(light_ref + ".specular_color", Dlight->specular_color);
          shader.setVec3(light_ref + ".direction", Dlight->direction);
          shader.setFloat(light_ref + ".intensity", Dlight->intensity);
        }

        dirCount++;
      }
      break;
    }
    case POINT: {
      if (pointCount < MAX_LIGHT_COUNT) {
        auto Plight = std::dynamic_pointer_cast<PointLight>(light);

        if (Plight) {
          std::string light_ref = "pointLights";
          light_ref += "[" + std::to_string(pointCount) + "]";
          shader.setBool(light_ref + ".light_active", true);
          shader.setVec3(light_ref + ".diffuse_color", Plight->diffuse_color);
          shader.setVec3(light_ref + ".specular_color", Plight->specular_color);
          shader.setVec3(light_ref + ".position", Plight->position);
          shader.setFloat(light_ref + ".intensity", Plight->intensity);
          shader.setFloat(light_ref + ".constant", 1.0f);
          shader.setFloat(light_ref + ".linear", Plight->linear);
          shader.setFloat(light_ref + ".quadratic", Plight->quadratic);
        }
        pointCount++;
      }
      break;
    }
    case SPOTLIGHT: {
      if (spotCount < MAX_LIGHT_COUNT) {
        auto Slight = std::dynamic_pointer_cast<SpotLight>(light);

        if (Slight) {
          std::string light_ref = "spotLights";
          light_ref += "[" + std::to_string(pointCount) + "]";
          shader.setBool(light_ref + ".light_active", true);
          shader.setVec3(light_ref + ".diffuse_color", Slight->diffuse_color);
          shader.setVec3(light_ref + ".specular_color", Slight->specular_color);
          shader.setVec3(light_ref + ".direction", Slight->direction);
          shader.setVec3(light_ref + ".position", Slight->position);
          shader.setFloat(light_ref + ".intensity", Slight->intensity);
          shader.setFloat(light_ref + ".inner_cutoff", Slight->inner_cutoff);
          shader.setFloat(light_ref + ".outer_cutoff", Slight->outer_cutoff);
        }
        spotCount++;
      }
      break;
    }
    case UNDEFINED: {
      break;
    }
    }
  }
}
void ERenderer::FetchShaderAndRender(const std::shared_ptr<EWorld> &world,
                                     std::shared_ptr<EObject3D> &obj) {

  switch (obj->getType()) {

  case EObject3D::Type::Mesh: {

    if (shader_map.find(obj) != shader_map.end()) {
      Shader *shader = shader_map[obj].get();
      if (window) {
        window->UpdateUniforms(*shader);
        shader->setBool("enableNormalMapping", normalMapping);
        window->UpdateUniforms(*outlineShader);
        CalculateLighting(world, *shader);

        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        obj->render(*shader);

        // disable writing to stencil and depth buffer
        glStencilMask(0x00);
        DisableDepthTesting();
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);

        if (outlinesEnabled) {
          auto scale = obj->getScale();
          obj->setScale(scale.x * outlineSize, scale.y * outlineSize,
                        scale.z * outlineSize);

          obj->render(*outlineShader);
          obj->setScale(scale.x, scale.y, scale.z);
        }
        glStencilMask(0xFF);
        EnableDepthTesting();
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
      }
    }
  }
  }
  for (auto &child : obj->getChildren()) {
    FetchShaderAndRender(world, child);
  }
}
void ERenderer::CompileModelShader(std::shared_ptr<EObject3D> &object) {
  auto model = std::dynamic_pointer_cast<EModel>(object);

  if (model && model->getMaterial()) {
    // HACK: this must be replaced
    std::string vertSrc = default_vertex_shader_source;
    std::string fragSrc = "";

    if (default_fragment_shader_sources.find(model->getMaterial()->type) !=
        default_fragment_shader_sources.end()) {
      fragSrc = default_fragment_shader_sources[model->getMaterial()->type];

      std::cout << "fragSrc: " << fragSrc << std::endl;
      std::string shader_src = vertSrc + fragSrc;
      if (compiled_shaders.find(shader_src) == compiled_shaders.end()) {

        auto cur_shader =
            std::make_shared<Shader>(vertSrc.c_str(), fragSrc.c_str());
        compiled_shaders[shader_src] = cur_shader;
        shader_map[model] = cur_shader;
      } else {

        shader_map[model] = compiled_shaders[shader_src];
      }
    } else if (model->getMaterial()->type == MaterialType_Shader) {

      auto material =
          std::dynamic_pointer_cast<ShaderMaterial>(model->getMaterial());

      if (material) {
        std::string shader_src =
            material->vertexShaderSrc + material->fragmentShaderSrc;
        if (compiled_shaders.find(shader_src) == compiled_shaders.end()) {
          auto cur_shader =
              std::make_shared<Shader>(material->vertexShaderSrc.c_str(),
                                       material->fragmentShaderSrc.c_str());
          compiled_shaders[shader_src] = cur_shader;
          shader_map[model] = cur_shader;
        } else {

          shader_map[model] = compiled_shaders[shader_src];
        }
      }
    }
  }
}
void ERenderer::addEffect(
    const std::shared_ptr<EPostProcessingEffect> &effect) {
  if (window) {
    effectPipeline.addEffect(effect);
  }
}
void ERenderer::addEfect(PostProcessingEffect effectType) {
  switch (effectType) {
  case PostProcessingEffect::Greyscale: {
    auto newEffect = std::make_shared<EGreyscaleEffect>();
    effectPipeline.addEffect(newEffect);
    break;
  }
  case PostProcessingEffect::Invert: {
    auto newEffect = std::make_shared<EInvertEffect>();
    effectPipeline.addEffect(newEffect);
    break;
  }
  case PostProcessingEffect::GaussianBlurHorizontal: {
    auto newEffect = std::make_shared<EGaussianBlurH>();
    effectPipeline.addEffect(newEffect);
    break;
  }
  case PostProcessingEffect::GaussianBlurVertical: {
    auto newEffect = std::make_shared<EGaussianBlurV>();
    effectPipeline.addEffect(newEffect);
    break;
  }
  case PostProcessingEffect::GaussianBlurFull: {
    auto Hblur = std::make_shared<EGaussianBlurH>();
    auto Vblur = std::make_shared<EGaussianBlurV>();
    effectPipeline.addEffect(Hblur);
    effectPipeline.addEffect(Vblur);
    break;
  }
  case PostProcessingEffect::Quantize: {
    auto newEffect = std::make_shared<EQuantization>();
    effectPipeline.addEffect(newEffect);
    break;
  }
  case PostProcessingEffect::Downsample: {
    auto newEffect = std::make_shared<EDownsample>();
    effectPipeline.addEffect(newEffect);
    break;
  }
  case PostProcessingEffect::Threshold: {
    auto newEffect = std::make_shared<EThreshold>();
    effectPipeline.addEffect(newEffect);
    break;
  }
  case PostProcessingEffect::Bloom: {
    auto newEffect =
        std::make_shared<EBloom>(window->getSize().w, window->getSize().h);
    effectPipeline.addEffect(newEffect);
    break;
  }
  }
}
