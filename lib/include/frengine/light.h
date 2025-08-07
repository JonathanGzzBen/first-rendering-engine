#ifndef LIGHT_H
#define LIGHT_H
#include <glm/vec3.hpp>

#include "renderable.h"

namespace frengine {
class ILight {
 public:
  virtual ~ILight() = default;

 public:
  virtual auto color() const -> glm::vec3 = 0;
};

class PointLight : public ILight {
 private:
  glm::vec3 color_;
  glm::vec3 position_;

 public:
  PointLight(const glm::vec3& position, const glm::vec3& color)
      : color_(color), position_(position) {}
  ~PointLight() override {}

  auto color() const -> glm::vec3 override { return color_; }
  auto position() const -> glm::vec3 { return position_; }
};

}  // namespace frengine

#endif  // LIGHT_H
