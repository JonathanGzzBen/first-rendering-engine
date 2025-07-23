#ifndef RENDERER_H
#define RENDERER_H

#include <expected>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <string>

#include "error.h"

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 uv;
};

namespace frengine {
class Renderer {
 private:
  explicit Renderer(unsigned int vao);

 public:
  static auto GetHelloWorld() -> std::string { return "Hello World"; }
  static auto Create() -> std::expected<Renderer, Error> {
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

    return Renderer(vao);
  }
};

}  // namespace frengine

#endif  // RENDERER_H
