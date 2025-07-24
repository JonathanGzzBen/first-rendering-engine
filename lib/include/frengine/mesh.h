#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>

#include <expected>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <vector>

#include "error.h"
#include "texture.h"

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 uv;
};

namespace frengine {
class Mesh {
 private:
  unsigned int vbo_;
  unsigned int ebo_;
  unsigned int indices_count_;
  std::vector<Texture> textures_;

 public:
  Mesh(const unsigned int vbo, const unsigned int ebo,
       const unsigned int indices_count_)
      : vbo_(vbo), ebo_(ebo), indices_count_(indices_count_) {}

  Mesh(const unsigned int vbo, const unsigned int ebo,
       const unsigned int indices_count_, const std::vector<Texture>& textures)
      : vbo_(vbo),
        ebo_(ebo),
        indices_count_(indices_count_),
        textures_(textures) {}

  ~Mesh() {
    glDeleteBuffers(1, &vbo_);
    glDeleteBuffers(1, &ebo_);
  }

  static auto Create(const std::vector<Vertex>& vertices,
                     const std::vector<unsigned int>& indices)
      -> std::expected<std::unique_ptr<Mesh>, Error> {
    unsigned int vbo;
    glCreateBuffers(1, &vbo);
    glNamedBufferStorage(vbo, vertices.size() * sizeof(Vertex), vertices.data(),
                         0);

    unsigned int ebo;
    glCreateBuffers(1, &ebo);
    glNamedBufferStorage(ebo, indices.size() * sizeof(unsigned int),
                         indices.data(), 0);

    return std::make_unique<Mesh>(vbo, ebo,
                                  static_cast<unsigned int>(indices.size()));
  }

  static auto Create(const std::vector<Vertex>& vertices,
                     const std::vector<unsigned int>& indices,
                     const std::vector<Texture>& textures)
      -> std::expected<std::unique_ptr<Mesh>, Error> {
    unsigned int vbo;
    glCreateBuffers(1, &vbo);
    glNamedBufferStorage(vbo, vertices.size() * sizeof(Vertex), vertices.data(),
                         0);

    unsigned int ebo;
    glCreateBuffers(1, &ebo);
    glNamedBufferStorage(ebo, indices.size() * sizeof(unsigned int),
                         indices.data(), 0);

    return std::make_unique<Mesh>(
        vbo, ebo, static_cast<unsigned int>(indices.size()), textures);
  }

  [[nodiscard]] auto vbo() const -> unsigned int { return vbo_; }
  [[nodiscard]] auto ebo() const -> unsigned int { return ebo_; }
  [[nodiscard]] auto indices_count() const -> unsigned int {
    return indices_count_;
  }
  [[nodiscard]] auto textures() const -> std::vector<Texture> {
    return textures_;
  }
};

}  // namespace frengine

#endif  // MESH_H
