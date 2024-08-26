#include "../include/EWorld.h"
#include <memory>

void EWorld::AddObject(std::shared_ptr<EModel> obj, Shader &shader) {

  std::pair<std::shared_ptr<EModel>, Shader &> shader_obj(obj, shader);
  Objects.push_back(shader_obj);
}
void EWorld::Render() {
  for (auto &[obj, shader] : Objects) {
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
    obj->Draw(shader);
  }
}

void EWorld::AddLight(std::shared_ptr<ELight> light) {
  Lights.push_back(light);
}
