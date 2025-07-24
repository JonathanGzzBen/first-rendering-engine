#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "renderable.h"

namespace frengine {

class Scene {
 private:
  std::vector<std::shared_ptr<IRenderable>> renderables_;

 public:
  auto AddRenderable(std::shared_ptr<IRenderable> renderable) -> void {
    renderables_.push_back(renderable);
  }

  const auto& GetRenderables() const { return renderables_; }
};

}  // namespace frengine
#endif  // SCENE_H
