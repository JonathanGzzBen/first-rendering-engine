#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <print>

#include "frengine/program.h"
#include "frengine/renderer.h"

auto glfw_error_callback(int error, const char *description) -> void {
  std::println(std::cerr, "GLFW error {}: {}", error, description);
}

void APIENTRY glDebugCallback(GLenum source, GLenum type, GLuint id,
                              GLenum severity, GLsizei length,
                              const GLchar *message, const void *userParam) {
  std::println(std::cerr, "glDebugCallback: {}: {}", type, message);
}

auto main() -> int {
  if (glfwInit() != GLFW_TRUE) {
    std::println(std::cerr, "Could not initialize glfw");
    return 1;
  }

  glfwSetErrorCallback(glfw_error_callback);

  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

  GLFWwindow *window =
      glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
  if (window == nullptr) {
    std::println(std::cerr, "Could not create GLFW window");
    glfwTerminate();
    return 1;
  }

  glfwMakeContextCurrent(window);

  if (glewInit() != GLEW_OK) {
    std::println(std::cerr, "Could not initialize GLEW");
    glfwTerminate();
    return 1;
  }

  // Configure OpenGL debugging if Debug context was created
  []() {
    int context_flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &context_flags);
    if (context_flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
      glEnable(GL_DEBUG_OUTPUT);
      glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
      glDebugMessageCallback(glDebugCallback, nullptr);
      glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0,
                            nullptr, GL_TRUE);
    }
  }();

  const auto program =
      frengine::Program::Create("shaders/vertex.glsl", "shaders/fragment.glsl");
  if (!program) {
    std::println(std::cerr, "Could not create program: {}",
                 program.error().message);
    glfwTerminate();
    return 1;
  }

  std::println("{}", frengine::Renderer::GetHelloWorld());
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  return 0;
}