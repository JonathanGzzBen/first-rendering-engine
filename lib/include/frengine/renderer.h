#ifndef RENDERER_H
#define RENDERER_H

#include <expected>
#include <string>

#include "error.h"

namespace frengine {
class Renderer {
 private:
  Renderer(unsigned int vao);

 public:
  static auto GetHelloWorld() -> std::string { return "Hello World"; }
  static auto Create() -> std::expected<Renderer, Error> { return Renderer(0); }
};

}  // namespace frengine

#endif  // RENDERER_H
