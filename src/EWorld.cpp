#include "../include/EWorld.h"

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
        shader.setVec3("ambientLight", light->ambient_color);
        shader.setFloat("ambientIntensity", light->intensity);
        break;
      }
      case DIRECTIONAL: {

        if (dirCount < MAX_LIGHT_COUNT) {
          auto Dlight = std::dynamic_pointer_cast<DirectionalLight>(light);
          std::string light_ref = "dirLights";
          /* std::cout << "Type: " << typeid(*light).name() << std::endl; */

          if (Dlight) {
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
        pointCount++;
        break;
      }
      case SPOTLIGHT: {
        spotCount++;
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
