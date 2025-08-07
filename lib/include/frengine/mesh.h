#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>

#include <expected>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <vector>

#include "error.h"
#include "material.h"
#include "renderable.h"
#include "texture.h"

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 uv;
};

namespace frengine {
class Mesh : public IRenderable {
 private:
  unsigned int vbo_;
  unsigned int ebo_;
  unsigned int indices_count_;
  Material material_;
  glm::mat4 model_matrix_ = glm::mat4(1.0F);

 public:
  Mesh(const unsigned int vbo, const unsigned int ebo,
       const unsigned int indices_count_)
      : vbo_(vbo),
        ebo_(ebo),
        indices_count_(indices_count_),
        material_(default_material) {}

  Mesh(const unsigned int vbo, const unsigned int ebo,
       const unsigned int indices_count_, const std::vector<Texture>& textures)
      : vbo_(vbo),
        ebo_(ebo),
        indices_count_(indices_count_),
        material_(default_material) {
    material_.textures = textures;
  }

  ~Mesh() override {
    glDeleteBuffers(1, &vbo_);
    glDeleteBuffers(1, &ebo_);
  }

  static auto Create(const std::vector<Vertex>& vertices,
                     const std::vector<unsigned int>& indices)
      -> std::expected<std::shared_ptr<Mesh>, Error> {
    unsigned int vbo;
    glCreateBuffers(1, &vbo);
    glNamedBufferStorage(vbo, vertices.size() * sizeof(Vertex), vertices.data(),
                         0);

    unsigned int ebo;
    glCreateBuffers(1, &ebo);
    glNamedBufferStorage(ebo, indices.size() * sizeof(unsigned int),
                         indices.data(), 0);

    return std::make_shared<Mesh>(vbo, ebo,
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
    return material_.textures;
  }
  [[nodiscard]] auto material() -> Material* { return &material_; }

  auto add_texture(const Texture& texture) -> void {
    material_.textures.emplace_back(texture);
  }

  // IRenderable
  auto Draw(const Program& program, const unsigned int vao) const
      -> void override {
    // Textures
    size_t diffuse_num = 1;
    for (size_t i = 0; i < material_.textures.size(); ++i) {
      material_.textures.at(i).Bind(i);

      std::string uniform_name{};
      if (material_.textures.at(i).Type() == Texture::Type::Diffuse) {
        uniform_name =
            "material.texture_diffuse_" + std::to_string(diffuse_num);
      }

      if (const auto res = program.Set1i(uniform_name, i); !res) {
        std::println(std::cerr, "Could not set {} uniform", uniform_name);
      }
    }

    if (const auto res =
            program.SetVec3("material.ambient_color", material_.ambient_color);
        !res) {
      std::println(std::cerr, "Could not set material.ambient_color uniform");
    }
    if (const auto res =
            program.SetVec3("material.diffuse_color", material_.diffuse_color);
        !res) {
      std::println(std::cerr, "Could not set material.diffuse_color uniform");
    }
    if (const auto res = program.SetVec3("material.specular_color",
                                         material_.specular_color);
        !res) {
      std::println(std::cerr, "Could not set material.specular_color uniform");
    }
    if (const auto res =
            program.Set1F("material.shininess", material_.shininess);
        !res) {
      std::println(std::cerr, "Could not set material.shininess uniform");
    }

    glVertexArrayVertexBuffer(vao, 0, vbo_, 0, sizeof(Vertex));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);

    glDrawElements(GL_TRIANGLES, static_cast<int>(indices_count_),
                   GL_UNSIGNED_INT, nullptr);
  }
};

}  // namespace frengine

#endif  // MESH_H
