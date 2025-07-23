#include <print>

#include "frengine/renderer.h"

auto main() -> int {
  std::println("{}", frengine::Renderer::GetHelloWorld());
  return 0;
}