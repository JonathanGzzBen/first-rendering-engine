#ifndef UNIFORM_SETTER_H
#define UNIFORM_SETTER_H
#include <expected>
#include <string>

#include "error.h"
#include "material.h"
#include "texture.h"

namespace frengine {

class UniformSetter {
 private:
  unsigned int program_;

 public:
  explicit UniformSetter(unsigned int program) : program_(program) {
    glUseProgram(program_);
  }

  auto SetMat4(const std::string& uniform_name, const glm::mat4& mat) const
      -> std::expected<UniformSetter, Error> {
    const auto location = glGetUniformLocation(program_, uniform_name.c_str());
    if (location == -1) {
      return std::unexpected(
          Error{.message = std::format("Could not get uniform location of '{}'",
                                       uniform_name)});
    }
    glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
    return {*this};
  }

  auto SetVec3(const std::string& uniform_name, const glm::vec3& vec) const
      -> std::expected<UniformSetter, Error> {
    const auto location = glGetUniformLocation(program_, uniform_name.c_str());
    if (location == -1) {
      return std::unexpected(
          Error{.message = std::format("Could not get uniform location of '{}'",
                                       uniform_name)});
    }
    glUniform3fv(location, 1, &vec[0]);
    return {*this};
  }

  auto Set1i(const std::string& uniform_name, const int value) const
      -> std::expected<UniformSetter, Error> {
    const auto location = glGetUniformLocation(program_, uniform_name.c_str());
    if (location == -1) {
      return std::unexpected(
          Error{.message = std::format("Could not get uniform location of '{}'",
                                       uniform_name)});
    }
    glUniform1i(location, value);
    return {*this};
  }
  auto Set1F(const std::string& uniform_name, const float value) const
      -> std::expected<UniformSetter, Error> {
    const auto location = glGetUniformLocation(program_, uniform_name.c_str());
    if (location == -1) {
      return std::unexpected(
          Error{.message = std::format("Could not get uniform location of '{}'",
                                       uniform_name)});
    }
    glUniform1f(location, value);
    return {*this};
  }

  auto SetMaterial(const std::string& uniform_name, const Material& value) const
      -> std::expected<UniformSetter, Error> {
    size_t diffuse_num = 1;
    for (size_t i = 0; i < value.textures.size(); ++i) {
      value.textures.at(i).Bind(i);

      std::string diffuse_texture_uniform_name{};
      if (value.textures.at(i).Type() == Texture::Type::Diffuse) {
        diffuse_texture_uniform_name =
            "material.texture_diffuse_" + std::to_string(diffuse_num);
      }

      if (const auto res = Set1i(diffuse_texture_uniform_name, i); !res) {
        return std::unexpected(
            Error{.message = std::format("Could not set uniform '{}'",
                                         diffuse_texture_uniform_name)});
      }
    }

    if (const auto res =
            this->SetVec3("material.ambient_color", value.ambient_color)
                .and_then([&value](const UniformSetter& b) {
                  return b.SetVec3("material.diffuse_color",
                                   value.diffuse_color);
                })
                .and_then([&value](const UniformSetter& b) {
                  return b.SetVec3("material.specular_color",
                                   value.specular_color);
                })
                .and_then([&value](const UniformSetter& b) {
                  return b.Set1F("material.shininess", value.shininess);
                });
        !res) {
      return std::unexpected(
          Error{.message = std::format("Could not set material: {}",
                                       res.error().message)});
    }

    return {*this};
  }
};

}  // namespace frengine

#endif  // UNIFORM_SETTER_H
