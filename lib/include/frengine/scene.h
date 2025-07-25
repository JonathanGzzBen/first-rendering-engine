#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "renderable.h"

namespace frengine {

struct Instance {
  std::shared_ptr<IRenderable> renderable;
  glm::mat4 transform;
};

class Scene {
 private:
  std::vector<Instance> instances_;

 public:
  auto AddInstance(const Instance& instance) -> void {
    instances_.emplace_back(instance);
  }

  const auto& GetInstances() const { return instances_; }
};

}  // namespace frengine
#endif  // SCENE_H
