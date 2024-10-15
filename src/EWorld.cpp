#include "../include/EWorld.h"
#include <cstdio>
#include <memory>

void EWorld::AddObject(std::shared_ptr<EModel> obj, Shader &shader) {

  std::pair<std::shared_ptr<EModel>, Shader &> shader_obj(obj, shader);
  Objects.push_back(shader_obj);
}
unsigned int EWorld::loadCubeMaps(std::vector<std::string> faces) {

  // the order should be
  // right, left, top, bottom, front, back
  float skyboxVertices[] = {
      // positions
      -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
      1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,

      -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
      -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

      1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

      -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

      -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

      -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
      1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f};
  // loading cube maps
  glGenTextures(1, &skyboxCubeMap);
  glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxCubeMap);

  int width, height, nchr;

  stbi_set_flip_vertically_on_load(false);
  for (int i = 0; i < (int)faces.size(); i++) {

    unsigned char *data =
        stbi_load(faces[i].c_str(), &width, &height, &nchr, 0);

    if (data) {
      unsigned int format;

      switch (nchr) {
      case 1: {
        format = GL_RED;
        break;
      }
      case 3: {
        format = GL_RGB;
        break;
      }
      case 4: {
        format = GL_RGBA;
        break;
      }
      }

      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height,
                   0, format, GL_UNSIGNED_BYTE, data);
      stbi_image_free(data);
    } else {
      fprintf(stderr, "Failed to load cube map data %s\n", faces[i].c_str());
      stbi_image_free(data);
    }
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  skyboxShader = std::make_shared<Shader>(
      Shader("../shaders/vertex/sykbox.glsl",
             "../shaders/fragment/skybox.glsl")); // generating skybox VAO
  unsigned int vbo;
  glGenVertexArrays(1, &skyboxVAO);
  glGenBuffers(1, &vbo);
  glBindVertexArray(skyboxVAO);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  hasSkyBox = true;
  return skyboxCubeMap;
}

void EWorld::RenderSkybox() {
  // disabling depth mapping so the sky box won't write anything to the depth
  // buffer

  if (skyboxShader) {
    glDepthMask(GL_FALSE);
    skyboxShader->Use();
    if (attachedWindow) {
      attachedWindow->UpdateUniforms(*skyboxShader);
      if (attachedWindow->getBoundCamera()) {
        glm::mat4 view =
            glm::mat4(glm::mat3(attachedWindow->getBoundCamera()->getLookAt()));
        skyboxShader->setMat4("uView", view);
      }
    }
    glBindVertexArray(skyboxVAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxCubeMap);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);
  }
}
void EWorld::Render() {

  for (auto &[obj, shader] : Objects) {
    if (attachedWindow) {
      attachedWindow->UpdateUniforms(shader);
    }
    unsigned int dirCount = 0;
    unsigned int pointCount = 0;
    unsigned int spotCount = 0;

    // defining shader uniforms for each light type
    for (auto &light : Lights) {
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
            shader.setVec3(light_ref + ".specular_color",
                           Dlight->specular_color);
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
            shader.setVec3(light_ref + ".specular_color",
                           Plight->specular_color);
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
            shader.setVec3(light_ref + ".specular_color",
                           Slight->specular_color);
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

    switch (obj->getType()) {
    case EObject3D::Type::Mesh: {
      auto mesh = std::dynamic_pointer_cast<EMesh>(obj);
      if (mesh) {
        mesh->render(shader);
      }
      break;
    }
    case EObject3D::Type::Model: {
      auto model = std::dynamic_pointer_cast<EModel>(obj);
      if (model) {
        model->render(shader);
      }
      break;
    }
    case EObject3D::Type::Group: {
      auto group = std::dynamic_pointer_cast<EGroup>(obj);
      if (group) {
        group->render(shader);
      }
      break;
    }
    }
  }

  if (hasSkyBox) {
    glDepthFunc(GL_LEQUAL);
    RenderSkybox();
    glDepthFunc(GL_LESS);
  }
}

void EWorld::AddLight(std::shared_ptr<ELight> light) {
  Lights.push_back(light);
}

void EWorld::render(Shader &shader) {}
