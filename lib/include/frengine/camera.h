#ifndef CAMERA_H
#define CAMERA_H
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/vec3.hpp>

namespace frengine {
class Camera {
 private:
  glm::vec3 position_;
  glm::vec3 front_;
  glm::vec3 up_;

  float pitch_;
  float yaw_;

 public:
  Camera(const glm::vec3 position, const glm::vec3 front, const glm::vec3 up)
      : position_(position),
        front_(front),
        up_(up),
        pitch_(0.0F),
        yaw_(-90.0F) {}

  auto GetViewMatrix() const -> glm::mat4 {
    return glm::lookAt(position_, position_ + front_, up_);
  }
  auto Position() const -> glm::vec3 { return position_; }
  auto Front() const -> glm::vec3 { return front_; }
  auto Up() const -> glm::vec3 { return up_; }

  auto Pan(const glm::vec3& direction) { position_ += direction; }
  auto AddPitch(const float pitch) {
    pitch_ += pitch;

    if (pitch_ >= 90.0f)
      pitch_ = 89.9f;
    else if (pitch_ <= -90.0f)
      pitch_ = -89.9f;

    glm::vec3 direction;
    direction.x = glm::cos(glm::radians(yaw_)) * glm::cos(glm::radians(pitch_));
    direction.y = glm::sin(glm::radians(pitch_));
    direction.z = glm::sin(glm::radians(yaw_)) * glm::cos(glm::radians(pitch_));
    front_ = glm::normalize(direction);
  }
  auto AddYaw(const float yaw) {
    yaw_ += yaw;

    glm::vec3 direction;
    direction.x = glm::cos(glm::radians(yaw_)) * glm::cos(glm::radians(pitch_));
    direction.y = glm::sin(glm::radians(pitch_));
    direction.z = glm::sin(glm::radians(yaw_)) * glm::cos(glm::radians(pitch_));
    front_ = glm::normalize(direction);
  }
};
}  // namespace frengine

#endif  // CAMERA_H
