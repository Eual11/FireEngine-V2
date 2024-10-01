#include "../include/EGroup.h"

EGroup::Type EGroup::getType() { return EGroup::Type::Group; }
void EGroup::render(Shader &shader) {
  for (auto &child : children) {
    child->render(shader);
  }
}
