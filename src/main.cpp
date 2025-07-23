#include <print>

#include "frengine/renderer.h"

auto main() -> int {
  std::println("{}", Renderer::GetHelloWorld());
  return 0;
}