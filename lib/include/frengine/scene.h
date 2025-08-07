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
  std::vector<ILight*> lights_;

 public:
  auto AddInstance(const Instance& instance) -> void {
    instances_.emplace_back(instance);
  }

  [[nodiscard]] const auto& GetInstances() const { return instances_; }
  [[nodiscard]] const auto& GetLights() const { return lights_; }

  auto AddLight(ILight* light) -> void { lights_.emplace_back(light); }
};

}  // namespace frengine
#endif  // SCENE_H
