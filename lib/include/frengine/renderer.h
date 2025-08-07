#ifndef RENDERER_H
#define RENDERER_H

#include <expected>

#include "error.h"
#include "mesh.h"
#include "model.h"
#include "program.h"
#include "scene.h"

namespace frengine {
class Renderer {
 private:
  unsigned int vao_;

 public:
  explicit Renderer(const unsigned int vao) : vao_(vao) {}

  ~Renderer() { glDeleteVertexArrays(1, &vao_); }

  static auto Create() -> std::expected<std::unique_ptr<Renderer>, Error> {
    unsigned int vao;
    glCreateVertexArrays(1, &vao);

    if (vao == 0) {
      return std::unexpected(Error{.message = "Could not create VAO"});
    }

    glEnableVertexArrayAttrib(vao, 0);
    glEnableVertexArrayAttrib(vao, 1);
    glEnableVertexArrayAttrib(vao, 2);

    glVertexArrayAttribBinding(vao, 0, 0);
    glVertexArrayAttribBinding(vao, 1, 0);
    glVertexArrayAttribBinding(vao, 2, 0);

    glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE,
                              offsetof(Vertex, position));
    glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE,
                              offsetof(Vertex, normal));
    glVertexArrayAttribFormat(vao, 2, 2, GL_FLOAT, GL_FALSE,
                              offsetof(Vertex, uv));

    return std::make_unique<Renderer>(vao);
  }

  auto RenderScene(const Scene& scene, const Program& program,
                   const glm::mat4& projection, const glm::mat4& view) const
      -> std::expected<void, Error> {
    program.Use();
    if (const auto res = program.SetMat4("projection", projection); !res) {
      return std::unexpected(
          Error{.message = std::format("Could not set projection matrix: {}",
                                       res.error().message)});
    }
    if (const auto res = program.SetMat4("view", view); !res) {
      return std::unexpected(Error{.message = "Could not set view matrix"});
    }

    glBindVertexArray(vao_);
    for (const auto& [renderable, transform] : scene.GetInstances()) {
      if (const auto res = program.SetMat4("model", transform); !res) {
        return std::unexpected(Error{.message = "Could not set view matrix"});
      }
      renderable->Draw(program, vao_);
    }

    static constexpr size_t max_point_lights = 100;
    size_t point_lights_count = 0;
    for (const auto& light : scene.GetLights()) {
      if (const auto point_light = dynamic_cast<PointLight*>(light)) {
        if (max_point_lights <= point_lights_count) {
          std::println(
              std::cerr,
              "Warning: max number of point lights reached. Ignoring light");
          continue;
        }
        if (const auto res = program.SetVec3(
                std::format("point_light_positions[{}]", point_lights_count),
                point_light->position());
            !res) {
          return std::unexpected(
              Error{.message = std::format("Could not set point light: {}",
                                           point_lights_count)});
        }
        point_lights_count++;
      }
    }

    glBindVertexArray(0);
    glUseProgram(0);
    return {};
  }
};

}  // namespace frengine

#endif  // RENDERER_H
