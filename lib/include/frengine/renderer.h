#ifndef RENDERER_H
#define RENDERER_H

#include <string>

class Renderer {
 public:
  static auto GetHelloWorld() -> std::string { return "Hello World"; }
};

#endif  // RENDERER_H
