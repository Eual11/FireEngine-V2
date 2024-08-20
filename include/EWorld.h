#ifndef _WORLD_H
#define _WORLD_H
#include "ELight.h"
#include "EModel.h"
#include "Shader.h"
#include <memory>
#include <vector>

#define MAX_LIGHT_COUNT 10
class EWorld {

private:
  std::vector<std::pair<std::shared_ptr<EModel>, Shader &>> Objects;
  std::vector<std::shared_ptr<ELight>> Lights;

public:
  EWorld() = default;
  void AddObject(std::shared_ptr<EModel>, Shader &);
  void AddLight(std::shared_ptr<ELight>);
  void Render();
};

#endif
