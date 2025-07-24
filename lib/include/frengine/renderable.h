#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "program.h"

namespace frengine {

class IRenderable {
 public:
  virtual ~IRenderable() = default;
  virtual auto Draw(const Program& program, const unsigned int vao) const
      -> void = 0;
  virtual void SetModelMatrix(const glm::mat4& model) = 0;
  virtual const glm::mat4& GetModelMatrix() const = 0;
};

}  // namespace frengine

#endif  // RENDERABLE_H
