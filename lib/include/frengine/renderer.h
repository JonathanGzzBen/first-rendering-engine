#ifndef RENDERER_H
#define RENDERER_H

#include <expected>
#include <string>

#include "error.h"
#include "mesh.h"

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

  auto Draw(const Program& program, const Mesh& mesh,
            const glm::mat4& projection_matrix,
            const glm::mat4& model_matrix) const -> std::expected<void, Error> {
    if (const auto res = program.SetMat4("projection", projection_matrix);
        !res) {
      return std::unexpected(
          Error{.message = std::format("Could not set projection matrix: {}",
                                       res.error().message)});
    }
    if (const auto res = program.SetMat4("model", model_matrix); !res) {
      return std::unexpected(Error{.message = "Could not set model matrix"});
    }

    program.Use();
    glBindVertexArray(vao_);
    glVertexArrayVertexBuffer(vao_, 0, mesh.vbo(), 0, sizeof(Vertex));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo());
    glDrawElements(GL_TRIANGLES, static_cast<int>(mesh.indices_count()),
                   GL_UNSIGNED_INT, nullptr);
    glUseProgram(0);
    return {};
  }
};

}  // namespace frengine

#endif  // RENDERER_H
