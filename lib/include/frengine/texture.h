#ifndef TEXTURE_H
#define TEXTURE_H

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

namespace frengine {
class Texture {
 public:
  enum class Type {
    Diffuse,
    Specular,
  };

 private:
  unsigned int texture_;
  Type type_;

 public:
  Texture(const unsigned int texture, Type type)
      : texture_(texture), type_(type) {}
  ~Texture() {
    // if (glIsTexture(texture_)) glDeleteTextures(1, &texture_);
  }

  static auto Create(const std::string& filename, Type type)
      -> std::expected<Texture, Error> {
    stbi_set_flip_vertically_on_load(true);
    int width, height, channels;
    unsigned char* data =
        stbi_load(filename.c_str(), &width, &height, &channels, 3);
    if (!data) {
      return std::unexpected(
          Error{.message = std::format("Could not load image data from '{}'",
                                       filename)});
    }

    unsigned int texture;
    glCreateTextures(GL_TEXTURE_2D, 1, &texture);
    if (!glIsTexture(texture)) {
      return std::unexpected(
          Error{.message = "Could not create texture object in OpenGL"});
    }
    glTextureStorage2D(texture, 1, GL_RGB8, width, height);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGB,
                        GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

    return Texture{texture, type};
  }

  auto Bind(const unsigned int texture_unit) const -> void {
    glBindTextureUnit(texture_unit, texture_);
  }

  auto Type() const -> Texture::Type { return type_; }
};

}  // namespace frengine

#endif  // TEXTURE_H
